.global contextSwitch

contextSwitch:
	mrs r12, cpsr
	push {r0-r14}

	str sp, [r0]
	ldr sp, [r1]

	pop {r0-r12}
	msr cpsr_c, r12
	pop {lr, pc}
