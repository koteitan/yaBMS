# Bashicu Matrix System

## What is a Bashicu Matrix?

A Bashicu Matrix is a notation system used to represent large ordinals (transfinite numbers). It consists of a sequence of tuples of natural numbers enclosed in parentheses.

### Examples:
- (0,0,0)(1,1,1)
- (0,0)(1,1)(2,1)

## Correspondence with Ordinals
Bashicu Matrices correspond to ordinals in a specific way. For example,
- (0) = 1
- (0)(0) = 2
- (0)(0)(0) = 3
- (0)(1) = omega
- (0)(1)(0) = omega + 1
- (0)(1)(1) = omega + 2
- (0)(1)(2) = omega + 3
- (0)(1)(0)(1) = omega 2
- (0)(1)(0)(1)(0) = omega 2 + 1
- (0)(1)(0)(1)(1) = omega 2 + 2
- (0)(1)(0)(1)(0)(1) = omega 3
- (0)(1)(1) = omega^2
- (0)(1)(1)(0) = omega^2 + 1
- (0)(1)(1)(0)(0) = omega^2 + 2
- (0)(1)(1)(0)(1) = omega^2 + omega
- (0)(1)(1)(0)(1)(0) = omega^2 + omega + 1
- (0)(1)(1)(0)(1)(0)(1) = omega^2 + omega 2
- (0)(1)(1)(0)(1)(1) = omega^22
- (0)(1)(1)(1) = omega^3
- (0)(1)(1)(1)(1) = omega^4
- (0)(1)(2) = omega^omega
- (0)(1)(2)(3) = omega^omega^omega
- (0,0)(1,1) = epsilon_0
- (0,0)(1,1)(0,0) = epsilon_0 + 1
- (0,0)(1,1)(1,0) = epsilon_0 omega
- (0,0)(1,1)(1,1) = epsilon_1
- (0,0)(1,1)(2,0) = epsilon_omega
- (0,0)(1,1)(2,1) = zeta_0
- (0,0)(1,1)(2,2) = psi_0(psi_2(0))
- (0,0)(1,1)(2,2)(0,0) = psi_0(psi_2(0)) + 1
- (0,0)(1,1)(2,2)(1,0) = psi_0(psi_2(0)+1)
- (0,0)(1,1)(2,2)(1,1) = psi_0(psi_2(0)+psi_1(0))
- (0,0)(1,1)(2,2)(2,0) = psi_0(psi_2(0)+psi_1(psi_2(0)+1))
- (0,0)(1,1)(2,2)(2,1) = psi_0(psi_2(0)+psi_1(psi_2(0)+psi_1(0)))
- (0,0)(1,1)(2,2)(2,2) = psi_0(psi_2(0)2)
- (0,0)(1,1)(2,2)(3,0) = psi_0(psi_2(1))
- (0,0)(1,1)(2,2)(3,1) = psi_0(psi_2(psi_1(0)))
- (0,0)(1,1)(2,2)(3,2) = psi_0(psi_2(psi_2(0)))
- (0,0)(1,1)(2,2)(3,3) = psi_0(psi_3(0))
- (0,0,0)(1,1,1) = psi_0(psi_omega(0))

## Fundamental Sequence of Bashicu Matrices
The fundamental sequence of a Bashicu Matrix M is a sequence of matrices that converge to the ordinal represented by M. For example, the fundamental sequence of (0,0)(1,1) = epsilon_0 is:
- (0,0) = 1
- (0,0)(1,0) = omega
- (0,0)(1,0)(2,0) = omega^omega
- (0,0)(1,0)(2,0)(3,0) = omega^omega^omega
- ...

## Expansion of Bashicu Matrices
The expanding a Bashicu Matrix M by n is denoted as M[n]. This operation is used to compute the nth element of the fundamental sequence of the ordinal represented by M.
For example,
- (0,0)(1,1)[0] = (0,0)
- (0,0)(1,1)[1] = (0,0)(1,0)
- (0,0)(1,1)[2] = (0,0)(1,0)(2,0)
- (0,0)(1,1)[3] = (0,0)(1,0)(2,0)(3,0)
- (0,0)(1,1)[4] = (0,0)(1,0)(2,0)(3,0)(4,0)

This expansion process is a core operation in working with Bashicu Matrices and is used to analyze properties of large ordinals.

Bashicu Matrix System (BMS) can be said to be a combination of the Bashicu Matrix Notation and the fundamental sequences for all Bashicu Matrices. 

# Usage
When you use the `expand_bms` tool, M[n] is the input string, and the output is the expanded form of the Bashicu Matrix. For example, if you input `(0,0)(1,1)[2]`, the output will be `(0,0)(1,0)(2,0)`.
