section .text
global L1
global RC4

L1:
	push	rsp
	push	0x0
	push	rbx
	push	rax
	push	rdi
	push	rsi
	push	rdx
	push	rcx
	push	r8
	push	r9
	push	r10
	push	r11
	push	r12
	push	r13
	push	r14
	push	r15

	nop				; BALISE
	nop				; BALISE

L2:
	lea	rdi, [ rel L1 ]		; KEY_ADDR
	mov	rsi, 0x59 		; KEY_SIZE
	lea	rdx, [ rel L1 ]		; TEX_ADDR
	mov	rcx, 0x42		; TEXT_SIZE

	call	RC4

	nop				; BALISE
	nop				; BALISE

	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	r11
	pop	r10
	pop	r9
	pop	r8
	pop	rcx
	pop	rdx
	pop	rsi
	pop	rdi
	pop	rax
	pop	rbx
	pop	rbp
	pop	rsp

	nop				; BALISE
	nop				; BALISE

	jmp	0xcafebabe

RC4:
	push	rbp
	mov	rbp, rsp

	nop				; BALISE
	nop				; BALISE

	sub	rsp, 0x188
	mov	r9, rdx
	mov	r8d, 0x0

R1:
	mov	byte [rsp+r8*1+0x88], r8b
	mov	eax, r8d
	cdq
	idiv	esi
	movsxd	rdx, edx
	movzx	eax, byte [rdi+rdx*1]
	mov	byte [rsp+r8*1-0x78],al	

	nop				; BALISE
	nop				; BALISE

	add	r8, 0x1
	cmp	r8,0x100
	jne	R1
	mov	edx, 0x0
	mov	esi, 0x0
	lea	r8, [rsp-0x78]

R2:
	nop				; BALISE
	nop				; BALISE

	movzx	edi, byte [rsp+rdx*1+0x88]
	movzx	eax, dil
	add	eax, esi
	movzx	esi, byte [rdx+r8*1]
	add	eax, esi
	mov	esi, eax
	sar	esi, 0x1f
	shr	esi, 0x18
	add	eax, esi

	nop				; BALISE
	nop				; BALISE

	movzx	eax,al
	sub	eax,esi
	mov	esi, eax
	cdqe
	movzx	r10d, byte [rsp+rax*1+0x88]
	mov	[rsp+rdx*1+0x88], r10b
	mov	[rsp+rax*1+0x88], dil

	nop				; BALISE
	nop				; BALISE

	add	rdx,0x1
	cmp	rdx,0x100
	jne	R2
	test	ecx,ecx
	jle	R3
	lea	eax, [rcx-0x1]
	lea	rdi, [r9+rax*1+0x1]
	xor	edx, edx
	xor	eax, eax

R4:
	add 	rax, 0x1
	movzx 	eax,al
	movzx 	ecx, byte [rsp+rax*1+0x88]
	add 	edx, ecx

	nop				; BALISE
	nop				; BALISE

	movzx 	edx,dl
	movzx 	esi, byte [rsp+rdx*1+0x88]
	mov	[rsp+rax*1+0x88], sil
	mov	[rsp+rdx*1+0x88], cl
	add	cl, [rsp+rax*1+0x88]
	xor	[r9],cl
	add	r9, 1
	cmp	rdi,r9
	jne	R4

R3:
	add	rsp,0x188

	nop				; BALISE
	nop				; BALISE

	leave
	ret
