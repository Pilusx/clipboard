.section .data
### CONSTANTS
.set MAX_BUF_SIZE, 1024
.set ALPHABET_SIZE, 26
.set LAST_ALPHA_IDX, 25 # (ALPHABET_SIZE - 1)
.set NUMBER_MAX_IDX, 9	# (10 - 1)
.set MODE_CIPHER, 0
.set MODE_DECIPHER, 1
.set PARSING, 0
.set NOT_PARSING, 1


.section .text

### FUNCTIONS CALLABLE IN C
# uint32_t cipher(char * buffer, uint32_t buffer_len, const char *key, uint32_t key_len, uint32_t mode);
.globl cipher

# int32_t copy_safely(const char * buffer, uint32_t* buffer_len, const char *key, uint32_t key_len);
.globl copy_safely

# uint32_t paste_safely(uint32_t id, char *buffer, uint32_t buffer_len, const char *key, uint32_t key_len);
.globl paste_safely

.macro zero reg
	xorl \reg, \reg
.endm


cipher:
        pushl   %ebp
        movl    $-1, %eax					# return -1
        pushl   %edi
        pushl   %esi
        pushl   %ebx
        subl    $1032, %esp
        movl    1064(%esp), %ebx			# ebx=buffer_len
        movl    1060(%esp), %ecx			# ecx=key[0]
        cmpl    $MAX_BUF_SIZE, %ebx
        ja      .L1 						# buffer_len > MAX_BUF_SIZE
        testl   %ebx, %ebx 
        je      .L3							# buffer_len <= MAX_BUF_SIZE
        zero	%eax
.L4:
        movzbl  (%ecx,%eax), %edx
        movb    %dl, 8(%esp,%eax)
        addl    $1, %eax
        cmpl    %eax, %ebx
        jne     .L4
        leal    8(%esp), %ecx
        zero 	%edx
        movl    $1, %esi
        movl    %ecx, %eax
        addl    %ebx, %eax
        zero	%ebx
        movl    %eax, (%esp)
        movl    %ebx, 1064(%esp)
        jmp     .L10
.L34:
		zero	%edi
        testl   %esi, %esi
        jne     .L6
        leal    (%edx,%edx,4), %edi
        addl    %edi, %edi
.L6:
        leal    -48(%edi,%eax), %esi
        movl    $1321528399, %edx
        movl    %esi, %eax
        mull    %edx
        shrl    $3, %edx
        imull   $ALPHABET_SIZE, %edx, %edx
        subl    %edx, %esi
        movl    %esi, %edx
        zero	%esi
.L7:
        addl    $1, %ecx
        cmpl    %ecx, (%esp)
        je      .L33
.L10:
        movsbl  (%ecx), %eax
        leal    -48(%eax), %edi
        movl    %edi, %ebx
        cmpb    $9, %bl
        jbe     .L34
        movl    1064(%esp), %edi
        movl    %edi, %ebx
        leal    8(%esp), %ebp
        addl    $1, %edi
        addl    %ebx, %ebp
        testl   %esi, %esi
        jne     .L8
        leal    8(%esp), %esi
        movb    %dl, 0(%ebp)
        leal    (%esi,%edi), %ebp
        movl    %ebx, %edi
        addl    $2, %edi
.L8:
        leal    -97(%eax), %ebx
        cmpb    $25, %bl
        ja      .L9
        movb    %bl, 0(%ebp)
        movl    $1, %esi
        addl    $1, %ecx
        movl    %edi, 1064(%esp)
        cmpl    %ecx, (%esp)
        jne     .L10
.L33:
        movl    1064(%esp), %ebx
        testl   %esi, %esi
        jne     .L3
        movb    %dl, 8(%esp,%ebx)
        addl    $1, %ebx
.L3:
        movl    1052(%esp), %esi
        movl    1056(%esp), %eax
        zero	%edx
        addl    %esi, %eax
        movl    %eax, 4(%esp)
        movl    1056(%esp), %eax
        testl   %eax, %eax
        je      .L16
        movl    %ebx, 1064(%esp)
        movl    1068(%esp), %edi
        jmp     .L15
.L35:
        leal    -97(%eax), %ecx
        cmpb    $LAST_ALPHA_IDX, %cl
        jbe     .L20
        leal    -65(%eax), %ecx
        cmpb    $LAST_ALPHA_IDX, %cl
        cmovbe  %ecx, %eax
        cmpb    $ALPHABET_SIZE, %cl
        sbbl    %ebx, %ebx
        andl    $65, %ebx
        movb    %bl, (%esp)
.L12:
        movzbl  8(%esp,%edx), %ebp
        movl    $ALPHABET_SIZE, %ecx
        movl    %ebp, %ebx
        subl    %ebx, %ecx
        addl    %eax, %ecx
        addl    %ebp, %eax
        cmpl    $1, %edi
        cmovne  %eax, %ecx
        addl    $1, %esi
        movsbw  %cl, %bp
        movl    %ecx, %ebx
        leal    0(%ebp,%ebp,4), %eax
        sarb    $7, %bl
        sall    $4, %eax
        subl    %ebp, %eax
        sarw    $11, %ax
        subl    %ebx, %eax
        movl    $ALPHABET_SIZE, %ebx
        imull   %ebx, %eax
        movzbl  (%esp), %ebx
        subl    %eax, %ecx
        leal    1(%edx), %eax
        zero	%edx
        addl    %ecx, %ebx
        divl    1064(%esp)
        movb    %bl, -1(%esi)
        cmpl    %esi, 4(%esp)
        je      .L16
.L15:
        movzbl  (%esi), %eax
        leal    -48(%eax), %ecx
        cmpb    $9, %cl
        ja      .L35
        movl    %ecx, %eax
        movb    $48, (%esp)
        jmp     .L12
.L9:
        subl    $65, %eax
        movl    %edi, 1064(%esp)
        movl    $1, %esi
        movb    %al, 0(%ebp)
        jmp     .L7
.L16:
		zero	%eax
.L1:
        addl    $1032, %esp
        popl    %ebx
        popl    %esi
        popl    %edi
        popl    %ebp
        ret
.L20:
        movl    %ecx, %eax
        movb    $97, (%esp)
        jmp     .L12

copy_safely:
        pushl   %ebp
        pushl   %edi
        pushl   %esi
        pushl   %ebx
        subl    $1068, %esp
        movl    1092(%esp), %eax
        movl    1088(%esp), %ebx
        movl    (%eax), %ecx
        cmpl    $MAX_BUF_SIZE, %ecx
        ja      .L51
        zero	%eax
        testl   %ecx, %ecx
        je      .L41
.L38:
        movzbl  (%ebx,%eax), %edx
        movb    %dl, 32(%esp,%eax)
        addl    $1, %eax
        cmpl    %eax, %ecx
        jne     .L38
.L41:
        pushl   $0
        pushl   1104(%esp)
        pushl   1104(%esp)
        pushl   %ecx
        leal    48(%esp), %eax
        pushl   %eax
        call    cipher
        movl    1112(%esp), %eax
        addl    $20, %esp
        movl    (%eax), %edi
        testl   %edi, %edi
        je      .L40
        leal    32(%esp), %ebp
        zero	%ecx
        movl    %ebp, %eax
        addl    %edi, %eax
        zero	%edi
        movl    %eax, 4(%esp)
        zero	%eax
.L47:
        movzbl  0(%ebp), %esi
        movl    %esi, %ebx
        cmpb    %al, %bl
        je      .L70
        testl   %ecx, %ecx
        je      .L53
        leal    1(%edi), %ebx
        movb    %al, 32(%esp,%edi)
        movl    %ebx, 8(%esp)
        cmpl    $1, %ecx
        je      .L54
        movl    %esi, %eax
        zero	%ebx
        movb    %al, 15(%esp)
        jmp     .L44
.L55:
        movl    %eax, %ebx
        movl    %edx, %ecx
.L44:
        movl    $-858993459, %eax
        movl    %ecx, %esi
        mull    %ecx
        shrl    $3, %edx
        leal    (%edx,%edx,4), %eax
        addl    %eax, %eax
        subl    %eax, %esi
        movl    %esi, %eax
        addl    $48, %eax
        movb    %al, 16(%esp,%ebx)
        leal    1(%ebx), %eax
        cmpl    $9, %ecx
        ja      .L55
        movzbl  15(%esp), %esi
        testl   %ebx, %ebx
        js      .L56
        leal    16(%esp), %eax
        leal    32(%esp), %edx
        movl    %ebx, (%esp)
        addl    8(%esp), %edx
        addl    %ebx, %eax
        jmp     .L46
.L57:
        movl    %ecx, %eax
.L46:
        movzbl  (%eax), %ecx
        leal    16(%esp), %ebx
        addl    $1, %edx
        movb    %cl, -1(%edx)
        leal    -1(%eax), %ecx
        cmpl    %ebx, %eax
        jne     .L57
        movl    (%esp), %ebx
        leal    2(%edi,%ebx), %edi
.L53:
        movl    %esi, %eax
        movl    $1, %ecx
.L43:
        addl    $1, %ebp
        cmpl    4(%esp), %ebp
        jne     .L47
        testl   %ecx, %ecx
        je      .L40
        leal    1(%edi), %esi
        movb    %al, 32(%esp,%edi)
        movl    %esi, 4(%esp)
        cmpl    $1, %ecx
        je      .L58
        zero	%ebx
        leal    16(%esp), %esi
        movl    $-858993459, %ebp
        movl    %edi, 8(%esp)
        jmp     .L48
.L59:
        movl    %eax, %ebx
        movl    %edx, %ecx
.L48:
        movl    %ecx, %eax
        movl    %ecx, %edi
        mull    %ebp
        shrl    $3, %edx
        leal    (%edx,%edx,4), %eax
        addl    %eax, %eax
        subl    %eax, %edi
        movl    %edi, %eax
        addl    $48, %eax
        movb    %al, (%esi,%ebx)
        leal    1(%ebx), %eax
        cmpl    $9, %ecx
        ja      .L59
        movl    8(%esp), %edi
        testl   %ebx, %ebx
        js      .L58
        leal    32(%esp), %ebp
        leal    (%esi,%ebx), %eax
        addl    4(%esp), %ebp
        jmp     .L50
.L61:
        movl    %edx, %eax
.L50:
        movzbl  (%eax), %edx
        addl    $1, %ebp
        movb    %dl, -1(%ebp)
        leal    -1(%eax), %edx
        cmpl    %eax, %esi
        jne     .L61
        leal    2(%edi,%ebx), %edi
.L40:
        subl    $8, %esp
        pushl   %edi
        leal    44(%esp), %eax
        pushl   %eax
        call    copy_to_clipboard
        movl    1108(%esp), %esi
        addl    $16, %esp
        movl    %edi, (%esi)
.L36:
        addl    $1068, %esp
        popl    %ebx
        popl    %esi
        popl    %edi
        popl    %ebp
        ret
.L70:
        addl    $1, %ecx
        jmp     .L43
.L54:
        movl    8(%esp), %edi
        movl    %esi, %eax
        jmp     .L43
.L56:
        movl    8(%esp), %edi
        jmp     .L53
.L58:
        movl    4(%esp), %edi
        jmp     .L40
.L51:
        movl    $-1, %eax
        jmp     .L36


paste_safely:
        pushl   %ebp
        pushl   %edi
        pushl   %esi
        pushl   %ebx
        subl    $1072, %esp
        movl    1100(%esp), %ebx
        leal    32(%esp), %eax
        movl    %ebx, 32(%esp)
        pushl   %eax
        leal    40(%esp), %esi
        pushl   %esi
        pushl   1100(%esp)
        call    paste_from_clipboard
        movl    %eax, 24(%esp)
        addl    $16, %esp
        testl   %eax, %eax
        jne     .L71
        testl   %ebx, %ebx
        je      .L73
        movsbl  32(%esp), %eax
        zero	%edx
        testb   %al, %al
        jne     .L77
        jmp     .L74
.L75:
        cmpb    $0, (%esi,%edx)
        je      .L74
.L77:
        addl    $1, %edx
        cmpl    %edx, %ebx
        jne     .L75
        movl    %ebx, %edx
.L74:
        movl    1092(%esp), %edi
        movl    1092(%esp), %ecx
        addl    %ebx, %edi
.L76:
        movb    $0, (%ecx)
        addl    $1, %ecx
        cmpl    %ecx, %edi
        jne     .L76
        testl   %edx, %edx
        je      .L78
        addl    %edx, %esi
        leal    33(%esp), %ecx
        zero	%edx
        movl    $1, %ebp
        movl    %esi, 4(%esp)
        zero	%edi
        zero	%esi
        movl    %ebx, 1096(%esp)
        jmp     .L86
.L110:
        testl   %edi, %edi
        jne     .L109
        movl    1092(%esp), %ebx
        movl    $1, %edi
.L80:
        movb    %al, (%ebx)
        movl    %eax, %esi
        cmpl    %ecx, 4(%esp)
        je      .L85
.L111:
        movsbl  (%ecx), %eax
        addl    $1, %ecx
.L86:
        leal    -48(%eax), %ebx
        cmpb    $9, %bl
        ja      .L110
        zero	%ebx
        cmpl    $1, %ebp
        je      .L84
        leal    (%edx,%edx,4), %ebx
        addl    %ebx, %ebx
.L84:
        leal    -48(%ebx,%eax), %ebx
        movl    $1321528399, %eax
        zero	%ebp
        mull    %ebx
        shrl    $3, %edx
        imull   $ALPHABET_SIZE, %edx, %edx
        subl    %edx, %ebx
        movl    %ebx, %edx
        cmpl    %ecx, 4(%esp)
        jne     .L111
.L85:
        movl    1096(%esp), %ebx
        cmpl    $1, %edx
        jbe     .L78
        movl    1092(%esp), %ecx
        movl    1092(%esp), %eax
        leal    -1(%ecx,%edi), %ecx
        addl    %edi, %eax
        addl    %ecx, %edx
        movl    %esi, %ecx
.L87:
        movb    %cl, (%eax)
        addl    $1, %eax
        cmpl    %edx, %eax
        jne     .L87
.L78:
        movl    1092(%esp), %esi
        zero	%eax
        movl    1092(%esp), %edx
        cmpb    $0, (%esi)
        jne     .L88
        jmp     .L112
.L114:
        cmpb    $0, (%edx,%eax)
        je      .L113
.L88:
        addl    $1, %eax
        cmpl    %eax, %ebx
        jne     .L114
.L73:
        subl    $12, %esp
        pushl   $1
        pushl   1120(%esp)
        pushl   1120(%esp)
        pushl   %ebx
        pushl   1120(%esp)
        call    cipher
        addl    $32, %esp
.L71:
        movl    8(%esp), %eax
        addl    $1068, %esp
        popl    %ebx
        popl    %esi
        popl    %edi
        popl    %ebp
        ret
.L109:
        movl    1092(%esp), %ebx
        addl    %edi, %ebx
        cmpl    $1, %edx
        jbe     .L115
        movl    1092(%esp), %ebp
        movb    %al, 15(%esp)
        movl    %esi, %eax
        movzbl  15(%esp), %esi
        leal    -1(%ebp,%edi), %ebp
        addl    %edx, %ebp
.L82:
        movb    %al, (%ebx)
        addl    $1, %ebx
        cmpl    %ebx, %ebp
        jne     .L82
        movl    %esi, %eax
        movl    1092(%esp), %esi
        addl    %edx, %edi
        movl    $1, %ebp
        zero	%edx
        leal    -1(%esi,%edi), %ebx
        jmp     .L80
.L113:
        movl    %eax, %ebx
        jmp     .L73
.L115:
        addl    $1, %edi
        movl    $1, %ebp
        zero	%edx
        jmp     .L80
.L112:
		zero	%ebx
        jmp     .L73
