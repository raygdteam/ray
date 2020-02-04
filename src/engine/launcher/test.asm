.code

Get_Ldr_Addr proc
    mov rax, GS:[30h]
    mov rax, [rax + 60h]
    ret
Get_Ldr_Addr endp

end