
Data SEGMENT 
    FIRST DB 00111111111b
A db -129
b db 0fach
    sec dw 0fffh
ccccc dw a2345h
    Th1 DD 12
c dd 123456789h
cc dd -15
ccc dd 010101010101010b
Data ENDS

Code segment

cc db -15
jmp begin
begin:
	jmp cc
	Stc
	Dec eax
	Dec bx
	Jz begin
	Pop ccc
	Pop dword ptr ds:[edx + esi]
Pop dword ptr [esp + edi
Pop dword ptr ds:[esp + esp]
Pop dword ptr ss:[esp + edi]
	Bsr al,bl
	jmp end_1
	And al,[ebx+ecx+eax]
	And edx,[ebx+ecx]
	And edx,ds:[ebx+ecx]
Add edx,[ebp+ecx]
And edx,ss:[ebp+ecx]
And edx,ds:[ebp+ecx]
	Or dword ptr [ebx + esi] , ecx
Mov al,bl
Mov al,cch
Mov edx,111110b
Mov al,-520
	Add dword ptr cs:[edx + ecx],0ffffffffh
Jz end_1
jmp begin
var dw 0fffh
cccc dd -15
Jz end_1
end_1:
Jz end_1
Code ENDS
END