#include "ARMv7AR.h"
#include "MemoryMap.h"

.text // .end까지의 모든 코드가 text 영역임을 의미.
    .code 32 // 명령어의 크기가 32비트.
    
        .global vector_start //label을 전역으로 설정하여 외부에서도 해당 label의 주소에 접근할 수 있도록 함.
        .global vector_end
        // 익셉션 벡터 테이블
        vector_start: 
            LDR PC, reset_handler_addr
            LDR PC, undef_handler_addr
            LDR PC, svc_handler_addr
            LDR PC, pftch_abt_handler_addr
            LDR PC, data_abt_handler_addr
            B   .
            LDR PC, irq_handler_addr
            LDR PC, fiq_handler_addr

            reset_handler_addr: .word reset_handler
            undef_handler_addr: .word dummy_handler
            svc_handler_addr: .word dummy_handler
            pftch_abt_handler_addr: .word dummy_handler
            data_abt_handler_addr: .word dummy_handler
            irq_handler_addr: .word dummy_handler
            fiq_handler_addr: .word dummy_handler
        vector_end:
        // 리셋 핸들러
        reset_handler:
            MRS r0,cpsr // move cpsr's flags in to Register 0
            BIC r1, r0, #0x1F // clear r0 bits (The flags)
            ORR r1,r1, #ARM_MODE_BIT_SVC //  DO OR 
            MSR cpsr, r1 // move register to status flags.(Set flags)
            LDR sp, =SVC_STACK_TOP // save address svc_stack_top to stack_pointer (update address)

            MRS r0,cpsr 
            BIC r1, r0, #0x1F 
            ORR r1,r1, #ARM_MODE_BIT_IRQ
            MSR cpsr, r1 
            LDR sp, =IRQ_STACK_TOP 

            MRS r0,cpsr 
            BIC r1, r0, #0x1F 
            ORR r1,r1, #ARM_MODE_BIT_FIQ
            MSR cpsr, r1 
            LDR sp, =FIQ_STACK_TOP

            MRS r0,cpsr 
            BIC r1, r0, #0x1F 
            ORR r1,r1, #ARM_MODE_BIT_ABT
            MSR cpsr, r1 
            LDR sp, =ABT_STACK_TOP

            MRS r0,cpsr 
            BIC r1, r0, #0x1F 
            ORR r1,r1, #ARM_MODE_BIT_UND
            MSR cpsr, r1 
            LDR sp, =UND_STACK_TOP

            MRS r0,cpsr 
            BIC r1, r0, #0x1F 
            ORR r1,r1, #ARM_MODE_BIT_SYS
            MSR cpsr, r1 
            LDR sp, =USRSYS_STACK_TOP

            dummy_handler:
                B   .
.end