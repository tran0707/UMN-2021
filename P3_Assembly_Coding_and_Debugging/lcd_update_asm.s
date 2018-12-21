.text
.global  set_tod_from_secs
        
## ENTRY POINT FOR REQUIRED FUNCTION
set_tod_from_secs:
        .cfi_startproc
####### COnditions check ##########
        cmpl    $86400, %edi                # edi = time_of_day_sec  
        jg      .RETURN_1                   # if time_of_day_sec > 86400
        cmpl    $0, %edi                   
        jl      .RETURN_1                   # if time_of_day_sec < 0

####### Calculate tod->hour #######        
        movl    %edi,%eax                   
        cqto                               
        movl    $3600,%ecx                 
        idivl   %ecx                        # time_of_day_sec/3600
        movw    %ax,0(%rsi)                 # store hour to 0(%rsi)

###### Calculate tod->minutes #######
        movl    %edx, %eax                  # take the remainder of the above division store in %eax
        cqto
        movl    $60,%r8d                    
        idivl   %r8d                        # remainder/60 = (time_of_day_sec - (tod->hour *3600))/60
        movw    %ax, 2(%rsi)                # store min to 2(%rsi)    

###### Calculate tod->seconds ######
        movw    %dx, 4(%rsi)                # remainder from minutes devision store to 4(%rsi) for secs
                                            # it equals to (time_of_day_sec -(tod->hour *3600)-(tod->minutes *60))
###### Conditions for tod-hours AM PM ######
        movq    0(%rsi),%rcx                # store hours to %rcx
        movb    $0,6(%rsi)                  # set to AM
        cmpw    $12,%cx                     
        jg      .HOURS_GREATER_12           # if (tod->hours > 12)
        cmpw    $0,%cx                      
        je      .HOURS_ZERO                 # if (hours = zero)
        cmpw    $11,%cx                     # using hours to determin the am/pm
        jg      .HOURS_GREATER_11           # if(tod->hour > 11)
        movl    $0,%eax
        ret

.RETURN_1: # if time_of_day_sec > 86400 and if time_of_day_sec < 0
        movl    $1,%eax                   # return 1 
        ret

.HOURS_GREATER_12: # if  tod->hours >12
        subw    $12,%cx                   # tod->hours = tod->hours - 12   
        movw    %cx,0(%rsi)
        movb    $1,6(%rsi)                # pm
        movl    $0,%eax
        ret

.HOURS_ZERO: # if tod->hours = 0
        movw    $12,0(%rsi)               # tod->hours = 12 
        movl    $0,%eax
        ret
        
.HOURS_GREATER_11: # if tod->hours > 11 (for AM/PM case)
        movb    $1,6(%rsi)                 # tod->ispm = 1 pm
        movl    $0,%eax
        ret
        .cfi_endproc

### Data area associated with the next function
.data
	
my_int:                                   
        .int    0                          # declare state = 0

my_array:                                  # declare my_array 
        .int    0b0111111                  
        .int    0b0000110                 
        .int    0b1011011
        .int    0b1001111
        .int    0b1100110
        .int    0b1101101
        .int    0b1111101
        .int    0b0000111
        .int    0b1111111
        .int    0b1101111
.text
.global  set_display_bits_from_tod

## ENTRY POINT FOR REQUIRED FUNCTION
set_display_bits_from_tod:
        .cfi_startproc
####### Conditions check hour ########
       movq     %rdi,%rcx                   # rdi = tod_t struct
       andq     $0xFFFF,%rcx                # %rcx = tod.hours
       cmpq     $0,%rcx
       jl       .RET_1                      # if tod.hour < 0
       cmpq     $12,%rcx
       jg       .RET_1                      # if tod.hour > 12
###### check conditions mins ##########
       movq     %rdi,%r8
       sarq     $16,%r8
       andq     $0xFFFF,%r8                 # %r8 = tod.minutes
       cmpq     $0,%r8
       jl       .RET_1                      # if tod.minutes < 0
       cmpq     $59,%r8
       jg       .RET_1                      # if tod.minutes > 59
####### check condition secs ##########
       movq     %rdi,%r9
       sarq     $32,%r9
       andq     $0xFFFF,%r9                 # %r9 = tod.seconds
       cmpq     $0,%r9
       jl       .RET_1                      # if tod.seconds < 0
       cmpq     $59,%r9
       jg       .RET_1                      # if tod.seconds > 59

####### Calculate one and tens ########
    ### min_one and ten ###
        movq    %r8,%rax
        cqto
        movq    $10,%r10
        idivq   %r10                 
        movq    %rdx, %r8                   # r8 will be min_one
        movq    %rax, %r10                  # r10 will be min_tens
    ### hour_one and ten ###    
        movq    %rcx,%rax
        cqto
        movq    $10,%r11
        idivq   %r11
        movq    %rdx,%rcx                   # rcx will be hour_one
        movq    %rax,%r11                   # r11 will be hour_ten

####### Shift and OR ##########
        leaq    my_array(%rip),%rdx         # load pointer to beginning of my_array into rdx
    ### min_one ###
        movq    (%rdx,%r8,4),%r8            # min_one = my_array[min_one]
        orl     %r8d,my_int(%rip)           # or bitwise state = state || min_one
    ### min_tens ###
        movq    (%rdx,%r10,4),%r10          # min_tens = my_array[min_tens]
        shll    $7,%r10d                    # min_tens << 7
        orl     %r10d,my_int(%rip)          # state = state | min_tens
    ### hour_one ###
        movq    (%rdx,%rcx,4),%rcx          # hour_one = my_array[hour_one]
        shll    $14,%ecx                    # hour_one << 14
        orl     %ecx,my_int(%rip)           # state = state | hour_one
    ### hour_ten ###
        cmpl     $1,%r11d                   
        je      .HOURS_TEN_1                # if (hour_tems == 1)
        jne     .HOURS_EMPTY                # else 

.HOURS_EMPTY: # if (hour_tens != 1)
        movl    $0,%r11d                    # hour_tens  = 0b0000000
        shll    $21,%r11d                   # hour_tens << 21
        orl     %r11d,my_int(%rip)          # state =state | hour_tens
        jmp     .AM_PM

.HOURS_TEN_1: # if (hour_tens == 1)
        movq    (%rdx,%r11,4),%r11          # hour_tens = my_array[hour_tens]
        shll    $21,%r11d                   # hour_tens << 21
        orl     %r11d,my_int(%rip)          # state = state | hour_tens
        jmp     .AM_PM

.AM_PM: 
        movq    %rdi,%rax
    	sarq    $48,%rax                    
	    andq	$0xFF,%rax                  # %rax = ispm
	    cmpq	$0, %rax
        je      .PM_0                       # if tod.ispm = 0
        jnz	    .PM_1                       # if tod.ispm = 1

.PM_0: # if tod.ispm = 0
        movq    $1,%rdx
        shlq    $28,%rdx    
        orl     %edx,my_int(%rip)           # state = state | ispm
	    jmp	    .RET_0

.PM_1: # if tod.ispm = 1
	    movq     $1,%rcx                    # copy 1 to %rcx = 0b0010
        shlq     $29,%rcx                   # shift left 29 = 0b0010 << 28
        orl      %ecx,my_int(%rip)          # state = state | ispm
        jmp     .RET_0

.RET_1: # if minutes,hours,seconds < 0 and hours > 12 and minutes,seconds > 59
        movq    $1,%rax                     # return 1
        ret

.RET_0:
	    movl	my_int(%rip),%ecx       
	    movl    %ecx,(%rsi) 
	    movl    $0,%eax
        ret       
	    .cfi_endproc

.text
.global lcd_update
lcd_update:
   #### calling set_tod_from_secs ####
    	.cfi_startproc
        movl    TIME_OF_DAY_SEC(%rip), %edi         # load global variable into eax
        cmpl    $86400,%edi
        jg      .RET_FAIL_1                         # if TIME_OF_DAY_SEC > 86400 
        cmpl    $0,%edi              
        jl      .RET_FAIL_1                         # if TIME_OF_DAY_SEC < 0
        pushq    %rdx                               # push %rdx on stack
        movq    %rsp,%rsi
        call    set_tod_from_secs
        cmpl    $1,%eax
        je      .RET_FAIL_1                         # if result of set_tod_from_secs return 1

   #### calling set_display_bits_from_tod ####
        movq    (%rsp),%rdi
        movl    $LCD_DISPLAY_PORT,%esi
        call    set_display_bits_from_tod
        cmpl    $1,%eax
        je     .RET_FAIL_1                          # if result of set_display_bits_from_tod return 1
        popq    %rdx
        movl    $0,%eax                             # return 0 for success
        ret
.RET_FAIL_1: 
        movl    $1,%eax
        ret
        .cfi_endproc










