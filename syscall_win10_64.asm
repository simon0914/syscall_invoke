;============================================
; syscall_.asm file for Windows 10 64 Bit
;============================================
; in part taken from https://gist.github.com/wbenny/b08ef73b35782a1f57069dff2327ee4d

  PUBLIC CallNtQueryInformationProcess
_TEXT SEGMENT
CallNtQueryInformationProcess PROC

  mov     r10, rcx
  mov     eax, 0000h  ; enter the correct syscall number for NtQueryInformationProcess
                      ; this depends on your version of windows:
                      ;   https://j00ru.vexillium.org/syscalls/nt/32/
                      ;   https://j00ru.vexillium.org/syscalls/nt/64/
  syscall
  ret

CallNtQueryInformationProcess ENDP
_TEXT ENDS
  END

; To compile this file use:
; ml64.exe syscall_win10_64.asm /c
