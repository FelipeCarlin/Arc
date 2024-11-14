start:
    cmpli 0, 0, 3, 0xFFFF  # Compare r3 (0xFFFF) with 0xFFFF in cr0, 32-bit
    cmpli 1, 0, 3, 0x1234  # Compare r3 (0xFFFF) with 0x1234 in cr1, 32-bit
    cmpli 2, 0, 4, 0x1234  # Compare r4 (0x1234) with 0x1234 in cr2, 32-bit
    cmpli 3, 0, 4, 0xFFFF  # Compare r4 (0x1234) with 0xFFFF in cr3, 32-bit
    cmpli 4, 0, 5, 0x8000  # Compare r5 (0x8000) with 0x8000 in cr4, 32-bit
    cmpli 5, 0, 6, 0x0     # Compare r6 (0) with 0 in cr5, 32-bit
    cmpli 6, 0, 6, 0xFFFF  # Compare r6 (0) with 0xFFFF in cr6, 32-bit
    cmpli 7, 0, 5, 0x7FFF  # Compare r5 (0x8000) with 0x7FFF in cr7, 32-bit
