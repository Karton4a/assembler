Data SEGMENT 
    FIRST DB 001111111b
A db -128
b db 0fah
    sec dw 0fffh
ccccc dw 01234h
    Th1 DD 12
c dd 12345678h
cc dd -15
ccc dd 010101010101010b
Data ENDS

Code segment
jmp begin
begin:
	jmp begin
	Stc
	Dec eax
	Dec bh
	Jz begin
	Pop dword ptr [edx + esi]
	Pop dword ptr ds:[edx + esi]
Pop dword ptr [esp + edi]
Pop dword ptr ds:[esp + edi]
Pop dword ptr ss:[esp + edi]
	Bsr eax,ebx
	jmp end_1
	And al,[ebx+ecx]
	And edx,[ebx+ecx]
	And edx,ds:[ebx+ecx]
And edx,[ebp+ecx]
And edx,ss:[ebp+ecx]
And edx,ds:[ebp+ecx]
	Or dword ptr [ebx + esi] , ecx
Mov al,0fh
Mov al,0ffh
Mov edx,111110b
Mov al,-5
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