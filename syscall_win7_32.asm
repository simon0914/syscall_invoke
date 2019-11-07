;============================================
; syscall_.asm file for Windows 7 32 Bit
;============================================
; in part taken from https://gist.github.com/wbenny/b08ef73b35782a1f57069dff2327ee4d

.586

  PUBLIC _CallNtQueryInformationProcess
_TEXT SEGMENT
_CallNtQueryInformationProcess PROC

  mov  eax, 0000h  ; enter the correct syscall number for NtQueryInformationProcess
                   ; this depends on your version of windows:
                   ;   https://j00ru.vexillium.org/syscalls/nt/32/
                   ;   https://j00ru.vexillium.org/syscalls/nt/64/
  mov  edx, 7FFE0300h
  call dword ptr [edx]

  ret

_CallNtQueryInformationProcess ENDP
_TEXT ENDS
  END

; To compile this file use:
; ml.exe syscall_win7_32.asm /c
