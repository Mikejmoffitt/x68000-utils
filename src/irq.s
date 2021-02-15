	.extern	g_xt_vbl_pending

	align 2
.global	g_irq_vbl

g_irq_vbl:
	clr.w	g_xt_vbl_pending
	rte
