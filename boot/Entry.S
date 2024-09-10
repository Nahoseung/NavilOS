.text // .end까지의 모든 코드가 text 영역임을 의미.
    .code 32 // 명령어의 크기가 32비트.
    
        .global vector_start //label을 전역으로 설정하여 외부에서도 해당 label의 주소에 접근할 수 있도록 함.
        .global vector_end

        vector_start: 
            LDR R0, =0x10000000
            LDR R1,[R0]
        vector_end:
        .space 1024, 0 //해당 위치에서 1024바이트를 0으로 채움.  
.end