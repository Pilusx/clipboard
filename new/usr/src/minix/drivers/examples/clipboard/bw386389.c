#include <minix/drivers.h>
#include <minix/chardriver.h>
#include <minix/ds.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "bw386389.h"

static clipboard_t CLIPBOARD;

/*
 * Function prototypes for the clipboard driver.
 */
static int clipboard_open(devminor_t minor, int access, endpoint_t user_endpt);
static int clipboard_close(devminor_t minor);
static ssize_t clipboard_read(devminor_t minor, u64_t position, endpoint_t endpt,
                              cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);
static ssize_t clipboard_write(devminor_t minor, u64_t position, endpoint_t endpt,
                               cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);


/* SEF functions and variables. */
static void sef_local_startup(void);
static int sef_cb_init(int type, sef_init_info_t *info);
static int sef_cb_lu_state_save(int);
static int lu_state_restore(void);

/* Entry points to the clipboard driver. */
static struct chardriver clipboard_tab =
{
    .cdr_open	= clipboard_open,
    .cdr_close	= clipboard_close,
    .cdr_read   = clipboard_read,
    .cdr_write  = clipboard_write
};


void delete_entry(uint32_t id) {
    assert(locked(id));
    push(id, &CLIPBOARD.available_entries);
    free(entry_buffer(id));
    slot(id) = EMPTY;
}


static int clipboard_open(devminor_t UNUSED(minor), int UNUSED(access),
                          endpoint_t UNUSED(user_endpt))
{
    log_msg("\n");
    return OK;
}

static int clipboard_close(devminor_t UNUSED(minor))
{
    log_msg("\n");
    return OK;
}

int get_lock_id(pid_t pid) {
    for(uint32_t id = 0; id < BUF_MAX_COUNT; id++) {
        if(entry(id).locking_pid == pid && locked(id)) {
            log_msg("Found lock on entry with id=%d pid=%d\n", id, pid);
            return id;
        }
    }
    log_msg("Pid=%d locks nothing.\n", pid);
    return -1;
}

void do_clipboard_real_set(int id, clipboard_set_msg_ptr msg) {
    log_msg("Used entry with id=%d\n", id);
    entry_buffer(id) = (char*) malloc(sizeof(char) * msg->buffer_len);
    memcpy(entry_buffer(id), msg->buffer, msg->buffer_len);
    entry(id).buffer_len = msg->buffer_len;
    entry(id).decompressed_len = msg->decompressed_len;
    entry(id).source_pid = msg->pid;
    slot(id) = USED;
}


int do_clipboard_set(clipboard_set_msg_ptr msg) {
    int id;

    if(is_empty(&CLIPBOARD.available_entries)) {
        log_msg("There are no available entries\n");
        return -1;
    }
    id = top(&CLIPBOARD.available_entries);
    pop(&CLIPBOARD.available_entries);
    if(busy(id) || locked(id)) {
        log_msg("ID is already in use... should panic\n");
        return -1;
    }

    do_clipboard_real_set(id, msg);

    return id;
}

int do_clipboard_lock(clipboard_lock_msg_ptr msg) {
    int id = get_lock_id(msg->pid);
    if(id >= 0) {
        log_msg("Unlocked previous locked entry with id=%d\n", id);
        unlock(id);
    }

    id = msg->id;

    if(slot(id) == LOCKED || !busy(id)) {
        log_msg("Current slot is not locked or is not in use.\n");
        return -1;
    }

    if(msg->buffer_len < entry(id).decompressed_len) {
        log_msg("Your buffer is too small to save the decompressed entry (%d < %d) \n", msg->buffer_len, entry(id).decompressed_len);
        return -1;
    }


    lock_by(id, msg->pid);
    log_msg("Sucessfully locked entry with id=%d pid=%d\n", id, msg->pid);
    return OK;
}

int do_clipboard_get(clipboard_get_msg_ptr msg, pid_t pid) {
    int id;
    if((id = get_lock_id(pid)) < 0) {
        return -1;
    }

    memcpy(msg->buffer, entry_buffer(id), entry_len(id));
    msg->decompressed_len = entry(id).decompressed_len;
    msg->buffer_len = entry_len(id);
    delete_entry(id);

    log_msg("Successfully copied entry with id=%d\n", id);
    return OK;
}


static ssize_t clipboard_read(devminor_t UNUSED(minor), u64_t UNUSED(position),
                              endpoint_t endpt, cp_grant_id_t grant, size_t size, int UNUSED(flags),
                              cdev_id_t UNUSED(id))
{
    int ret;
    int id;
    int pid;
    int rsize;
    if(IS_PID_QUERY(size)) {
        id = DECODE_ID(size);
        if(!IS_CORRECT_ID(id)) {
            log_msg("Wrong id [PID QUERY]\n");
            return -1;
        }
        else {
            pid = entry(id).source_pid;
            log_msg("Got pid %d [PID QUERY] from entry with id=%d\n", pid, id);
            return pid;
        }
    }

    rsize = GET_SIZE;
    pid = DECODE_PID(size);
    log_msg("decoded pid as %d\n", pid);
    struct clipboard_get_msg msg;

    log_msg("\n");
    if(pid < 0) {
        log_msg("Wrong pid %d\n", pid);
        return -1;
    }

    if ((ret = do_clipboard_get(&msg, pid) != OK))
        return ret;

    if (((ret = sys_safecopyto(endpt, grant, 0, (vir_bytes) &msg, rsize))) != OK)
        return ret;

    return OK;
}

static ssize_t clipboard_write(devminor_t UNUSED(minor), u64_t UNUSED(position),
                               endpoint_t endpt, cp_grant_id_t grant, size_t size, int UNUSED(flags),
                               cdev_id_t UNUSED(id))
{
    assert(LOCK_SIZE != GET_SIZE);
    int ret;
    struct clipboard_write_msg msg;
    log_msg("\n");

    if(size == LOCK_SIZE || size == SET_SIZE) {
        if ((ret = sys_safecopyfrom(endpt, grant, 0, (vir_bytes) &msg, size)) != OK)
            return ret;
    }
    else {
        log_msg("Wrong size %d != %d\n", sizeof(msg), size);
        return -1;
    }

    if(size == LOCK_SIZE) {
        return do_clipboard_lock(&msg.u.lock);
    }
    else if(size == SET_SIZE) {
        return do_clipboard_set(&msg.u.set);
    }

    return -1;
}

#ifdef DEBUG
void log_string(const char* c, uint32_t len) {
    for(int i = 0; i < len; i++)
        printf("%c", c[i]);

    printf(" |LEN=%d| ", len);
}
#endif

void clipboard_log() {
#ifdef DEBUG
    static int k = 0;

    log_msg("LOG %d \n", k++);
    for(int i = 0; i < BUF_MAX_COUNT; i++) {
        if(busy(i)) {
            printf("%d (LE=%d, DE=%d):  ", i, entry_len(i), entry(i).decompressed_len);
            log_string(entry_buffer(i), entry_len(i));
            printf("\n");
        }
    }
#endif
}

void clipboard_init() {
    init(&CLIPBOARD.available_entries);
    for(int i = 0; i < BUF_MAX_COUNT; i++) {
        slot(i) = EMPTY;
    }
}

static int sef_cb_lu_state_save(int UNUSED(state)) {
    /* Save the state. */
    char name[16] = "buf_";
    uint32_t i;

    log_msg("Current state:\n");
    clipboard_log();
    ds_publish_mem("clipboard",(char*) &CLIPBOARD, sizeof(clipboard_t), DSF_OVERWRITE);
    for(i = 0; i < BUF_MAX_COUNT; i++) {
        if(slot(i) != EMPTY) {
            snprintf(name + 4, 12, "%d", i);
            log_msg("Saves %s\n", name);
            ds_publish_mem(name, (char*) entry_buffer(i), entry_len(i), DSF_OVERWRITE);
            free(entry_buffer(i));
        }
    }
    return OK;
}

static int lu_state_restore() {
    /* Restore the state. */
    char name[16] = "buf_";
    uint32_t size;
    uint32_t i;

    size = sizeof(clipboard_t);
    ds_retrieve_mem("clipboard",(char*) &CLIPBOARD, &size);
    ds_delete_mem("clipboard");

    log_msg("restored state:\n");
    for(i = 0; i < BUF_MAX_COUNT; i++) {
        if(slot(i) != EMPTY) {
            snprintf(name + 4, 12, "%d", i);
            entry_buffer(i) = (char*) malloc(sizeof(char) * entry_len(i));
            log_msg("Restores %s\n", name);
            size = sizeof(char) * entry_len(i);
            ds_retrieve_mem(name, (char*) entry_buffer(i), &size);
            ds_delete_mem(name);
        }
    }
    clipboard_log();
    return OK;
}

static void sef_local_startup()
{
    /*
     * Register init callbacks. Use the same function for all event types
     */
    sef_setcb_init_fresh(sef_cb_init);
    sef_setcb_init_lu(sef_cb_init);
    sef_setcb_init_restart(sef_cb_init);

    /*
     * Register live update callbacks.
     */
    /* - Agree to update immediately when LU is requested in a valid state. */
    sef_setcb_lu_prepare(sef_cb_lu_prepare_always_ready);
    /* - Support live update starting from any standard state. */
    sef_setcb_lu_state_isvalid(sef_cb_lu_state_isvalid_standard);
    /* - Register a custom routine to save the state. */
    sef_setcb_lu_state_save(sef_cb_lu_state_save);

    /* Let SEF perform startup. */
    sef_startup();
}

static int sef_cb_init(int type, sef_init_info_t *UNUSED(info))
{
    switch(type) {
    case SEF_INIT_FRESH:
        log_msg("SEF_INIT_FRESH\n");
        clipboard_init();
        break;

    case SEF_INIT_LU:
        /* Restore the state. */
        lu_state_restore();
        log_msg("SEF_INIT_LU Hey, I'm a new version!\n");
        break;

    case SEF_INIT_RESTART:
        log_msg("SEF_INIT_RESTART Hey, I've just been restarted!\n");
        break;
    }

    /* Initialization completed successfully. */
    return OK;
}


int main(void)
{
    /*
     * Perform initialization.
     */
    sef_local_startup();

    /*
     * Run the main loop.
     */
    chardriver_task(&clipboard_tab);
    return OK;
}

