val1: add $1,$2,$3
 add $1,$2,$3
		call	val1
		jmp		$4  
END:	stop
STR:	.asciz	"aBcd"
LIST:	.db		6,-9
		.dh		27056
.entry	K
K:		.dw		31,-12
.extern	val1