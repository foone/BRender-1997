/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: matrix23.c 2.5 1996/12/04 15:42:32 OWAIN Exp $
 * $Locker: $
 *
 * A very unoptimised set of transforms - these should each
 * be re-done for the sparse multiplication.
 *
 * 3x4 Matrices
 *
 */

#include "brender.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: matrix23.c 2.5 1996/12/04 15:42:32 OWAIN Exp $")

static br_matrix23 mattmp1,mattmp2;

/*
 * A = B
 */
void BR_PUBLIC_ENTRY BrMatrix23Copy(br_matrix23 *A, br_matrix23 *B)
{
        UASSERT_MESSAGE("Destination matrix is NULL", A != NULL);
        UASSERT_MESSAGE("Source matrix is NULL", B != NULL);

        A(0,0) = B(0,0);
	A(0,1) = B(0,1);

	A(1,0) = B(1,0);
	A(1,1) = B(1,1);

	A(2,0) = B(2,0);
	A(2,1) = B(2,1);
}

/*
 * A = B*C
 */
void BR_PUBLIC_ENTRY BrMatrix23Mul(br_matrix23 *A, br_matrix23 *B, br_matrix23 *C)
{
        UASSERT_MESSAGE("Destination matrix is NULL", A != NULL);
        UASSERT_MESSAGE("Left Hand Source matrix is NULL", B != NULL);
        UASSERT_MESSAGE("Right Hand Source matrix is NULL", C != NULL);

	A(0,0) = BR_MAC2(B(0,0),C(0,0), B(0,1),C(1,0));
	A(0,1) = BR_MAC2(B(0,0),C(0,1), B(0,1),C(1,1));

	A(1,0) = BR_MAC2(B(1,0),C(0,0), B(1,1),C(1,0));
	A(1,1) = BR_MAC2(B(1,0),C(0,1), B(1,1),C(1,1));

	A(2,0) = BR_MAC2(B(2,0),C(0,0), B(2,1),C(1,0)) + C(2,0);
	A(2,1) = BR_MAC2(B(2,0),C(0,1), B(2,1),C(1,1)) + C(2,1);

}

void BR_PUBLIC_ENTRY BrMatrix23Identity(br_matrix23 *mat)
{
        UASSERT_MESSAGE("Destination matrix is NULL", mat != NULL);

        M(0,0) = S1; M(0,1) = S0;
	M(1,0) = S0; M(1,1) = S1;
	M(2,0) = S0; M(2,1) = S0;
}

/*
 *         �                �
 *         �  cos�  sin�  0 �
 *         �                �
 *         � -sin�  cos�  0 �
 * R(�) =  �                �
 *         �   0     0    1 �
 *         �                �
 */
void BR_PUBLIC_ENTRY BrMatrix23Rotate(br_matrix23 *mat, br_angle rz)
{
	br_scalar s = BR_SIN(rz);
	br_scalar c = BR_COS(rz);

        UASSERT_MESSAGE("Destination matrix is NULL", mat != NULL);

	M(0,0) =  c; M(0,1) =  s;
	M(1,0) = -s; M(1,1) =  c;
	M(2,0) = S0; M(2,1) = S0;
}

/*
 *            �             �
 *            �  1   0   0  �
 *            �             �
 *            �  0   1   0  �
 * T(dx,dy) = �             �
 *            �  dx  dy  1  �
 *            �             �
 */
void BR_PUBLIC_ENTRY BrMatrix23Translate(br_matrix23 *mat, br_scalar dx, br_scalar dy)
{
        UASSERT_MESSAGE("Destination matrix is NULL", mat != NULL);

        M(0,0) = S1; M(0,1) = S0;
	M(1,0) = S0; M(1,1) = S1; 
	M(2,0) = dx; M(2,1) = dy; 
}

/*
 *            �             �
 *            �  sx  0    0 �
 *            �             �
 *            �  0   sy   0 �
 * S(sx,sy) = �             �
 *            �  0   0    1 �
 *            �             �
 */
void BR_PUBLIC_ENTRY BrMatrix23Scale(br_matrix23 *mat, br_scalar sx, br_scalar sy)
{
        UASSERT_MESSAGE("Destination matrix is NULL", mat != NULL);

	M(0,0) = sx; M(0,1) = S0;
	M(1,0) = S0; M(1,1) = sy;
	M(2,0) = S0; M(2,1) = S0;
}

/*
 *              �             �
 *              �  1   sy   0 �
 *              �             �
 *              �  0   1    0 �
 * ShearX(sy) = �             �
 *              �  0   0    1 �
 *              �             �
 */
void BR_PUBLIC_ENTRY BrMatrix23ShearX(br_matrix23 *mat, br_scalar sy)
{
        UASSERT_MESSAGE("Destination matrix is NULL", mat != NULL);

        M(0,0) = S1; M(0,1) = sy;
	M(1,0) = S0; M(1,1) = S1;
	M(2,0) = S0; M(2,1) = S0;
}

/*
 *              �             �
 *              �  1   0    0 �
 *              �             �
 *              �  sx  1    0 �
 * ShearY(sx) = �             �
 *              �  0   0    1 �
 *              �             �
 */
void BR_PUBLIC_ENTRY BrMatrix23ShearY(br_matrix23 *mat, br_scalar sx)
{
        UASSERT_MESSAGE("Destination matrix is NULL", mat != NULL);

        M(0,0) = S1; M(0,1) = S0;
	M(1,0) = sx; M(1,1) = S1;
	M(2,0) = S0; M(2,1) = S0;
}

/*
 * Computes the inverse of a 2D affine matrix; 
 */

#define PRECISION_LIMIT BR_SCALAR(1e-15)

br_scalar BR_PUBLIC_ENTRY BrMatrix23Inverse(br_matrix23 *B, br_matrix23 *A)
{
    br_scalar det,idet,pos = 0,neg = 0;

    UASSERT_MESSAGE("Destination matrix is NULL", A != NULL);
    UASSERT_MESSAGE("Source matrix is NULL", B != NULL);
    UASSERT_MESSAGE("Source may not be destination", A != B);

    det = BR_MUL(A(0,0),A(1,1));
    if (det>=0)
	pos += det;
    else
	neg += det;

    det = BR_MUL(-A(1,0),A(0,1));
    if (det>=0)
	pos += det;
    else
	neg += det;

    det = pos+neg;

    if (BR_ABS(det)<=BR_SCALAR_EPSILON*2)
	return S0;

    if (BR_DIV(det,pos-neg)<PRECISION_LIMIT)
	return S0;

    idet = BR_RCP(det);

    B(0,0) = BR_MUL(A(1,1),idet);
    B(0,1) = -BR_MUL(A(0,1),idet);
    B(1,0) = -BR_MUL(A(1,0),idet);
    B(1,1) = BR_MUL(A(0,0),idet);
    B(2,0) = BR_MUL(BR_MUL(A(1,0),A(2,1))-BR_MUL(A(1,1),A(2,0)),idet);
    B(2,1) = BR_MUL(BR_MUL(A(0,1),A(2,0))-BR_MUL(A(0,0),A(2,1)),idet);

    return det;
}

/*
 * Invert a length preserving matrix
 */

void BR_PUBLIC_ENTRY BrMatrix23LPInverse(br_matrix23 *B, br_matrix23 *A)
{
    UASSERT_MESSAGE("Destination matrix is NULL", A != NULL);
    UASSERT_MESSAGE("Source matrix is NULL", B != NULL);
    UASSERT_MESSAGE("Source may not be destination", A != B);

    B(0,0) = A(1,1);
    B(0,1) = -A(0,1);
    B(1,0) = -A(1,0);
    B(1,1) = A(0,0);
    B(2,0) = BR_MUL(A(1,0),A(2,1))-BR_MUL(A(1,1),A(2,0));
    B(2,1) = BR_MUL(A(0,1),A(2,0))-BR_MUL(A(0,0),A(2,1));
}

void BR_PUBLIC_ENTRY BrMatrix23LPNormalise(br_matrix23 *A, br_matrix23 *B)
{
        UASSERT_MESSAGE("Destination matrix is NULL", A != NULL);
        UASSERT_MESSAGE("Source matrix is NULL", B != NULL);

	BrVector2Normalise((br_vector2 *)A->m[Y],(br_vector2 *)B->m[Y]);
	A(0,0) = A(1,1);
	A(0,1) = -A(1,0);

	A(2,0) = B(2,0);
	A(2,1) = B(2,1);
}

/*
 * [a b ] = [ e f ] . M
 */
void BR_PUBLIC_ENTRY BrMatrix23ApplyP(br_vector2 *A, br_vector2 *B, br_matrix23 *C)
{
        UASSERT_MESSAGE("Destination vector is NULL", A != NULL);
        UASSERT_MESSAGE("Source vector is NULL", B != NULL);
        UASSERT_MESSAGE("Transform matrix is NULL", C != NULL);
        UASSERT_MESSAGE("Source may not be Destination", A != B);

        A->v[0] = BR_MAC2(B->v[0],C(0,0), B->v[1],C(1,0)) + C(2,0);
	A->v[1] = BR_MAC2(B->v[0],C(0,1), B->v[1],C(1,1)) + C(2,1);
}

/*
 * [a b c] = [ e f 0 ] . M
 */
void BR_PUBLIC_ENTRY BrMatrix23ApplyV(br_vector2 *A, br_vector2 *B, br_matrix23 *C)
{
        UASSERT_MESSAGE("Destination vector is NULL", A != NULL);
        UASSERT_MESSAGE("Source vector is NULL", B != NULL);
        UASSERT_MESSAGE("Transform matrix is NULL", C != NULL);
        UASSERT_MESSAGE("Source may not be Destination", A != B);

        A->v[0] = BR_MAC2(B->v[0],C(0,0), B->v[1],C(1,0));
	A->v[1] = BR_MAC2(B->v[0],C(0,1), B->v[1],C(1,1));
}

/*
 * [a b] = [ e f] . transpose(M)
 */
void BR_PUBLIC_ENTRY BrMatrix23TApplyP(br_vector2 *A, br_vector2 *B, br_matrix23 *C)
{
        UASSERT_MESSAGE("Destination vector is NULL", A != NULL);
        UASSERT_MESSAGE("Source vector is NULL", B != NULL);
        UASSERT_MESSAGE("Transform matrix is NULL", C != NULL);
        UASSERT_MESSAGE("Source may not be Destination", A != B);

	A->v[0] = BR_MAC2(B->v[0],C(0,0), B->v[1],C(0,1));
	A->v[1] = BR_MAC2(B->v[0],C(1,0), B->v[1],C(1,1));
}

/*
 * [a b c] = [ e f 0 ] . transpose(M)
 */
void BR_PUBLIC_ENTRY BrMatrix23TApplyV(br_vector2 *A, br_vector2 *B, br_matrix23 *C)
{
        UASSERT_MESSAGE("Destination vector is NULL", A != NULL);
        UASSERT_MESSAGE("Source vector is NULL", B != NULL);
        UASSERT_MESSAGE("Transform matrix is NULL", C != NULL);
        UASSERT_MESSAGE("Source may not be Destination", A != B);

        A->v[0] = BR_MAC2(B->v[0],C(0,0), B->v[1],C(0,1));
	A->v[1] = BR_MAC2(B->v[0],C(1,0), B->v[1],C(1,1));
}

/*
 * Composite matrix operations -
 * pre and post-multiply with an existing matrix
 */

void BR_PUBLIC_ENTRY BrMatrix23Pre(br_matrix23 *mat , br_matrix23 *A)
{
        UASSERT_MESSAGE("Subject matrix is NULL", mat != NULL);
        UASSERT_MESSAGE("Pre-multiplying matrix is NULL", A != NULL);
        
        BrMatrix23Mul(&mattmp2,A,mat);
	BrMatrix23Copy(mat,&mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix23Post(br_matrix23 *mat , br_matrix23 *A)
{
        UASSERT_MESSAGE("Subject matrix is NULL", mat != NULL);
        UASSERT_MESSAGE("Post-multiplying matrix is NULL", A != NULL);

        BrMatrix23Mul(&mattmp2,mat,A);
	BrMatrix23Copy(mat,&mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix23PreRotate(br_matrix23 *mat, br_angle rz)
{			  
        UASSERT_MESSAGE("Matrix is NULL", mat != NULL);

	BrMatrix23Rotate(&mattmp1,rz);
	BrMatrix23Mul(&mattmp2,&mattmp1,mat);
	BrMatrix23Copy(mat,&mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix23PostRotate(br_matrix23 *mat, br_angle rz)
{
        UASSERT_MESSAGE("Matrix is NULL", mat != NULL);

	BrMatrix23Rotate(&mattmp1,rz);
	BrMatrix23Mul(&mattmp2,mat,&mattmp1);
	BrMatrix23Copy(mat,&mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix23PreTranslate(br_matrix23 *mat, br_scalar x, br_scalar y)
{
        UASSERT_MESSAGE("Matrix is NULL", mat != NULL);

	BrMatrix23Translate(&mattmp1,x,y);
	BrMatrix23Mul(&mattmp2,&mattmp1,mat);
	BrMatrix23Copy(mat,&mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix23PostTranslate(br_matrix23 *A, br_scalar x, br_scalar y)
{
        UASSERT_MESSAGE("Matrix is NULL", A != NULL);

	A(2,0) += x;
	A(2,1) += y;
}

void BR_PUBLIC_ENTRY BrMatrix23PreScale(br_matrix23 *mat, br_scalar sx, br_scalar sy)
{
        UASSERT_MESSAGE("Matrix is NULL", mat != NULL);

	BrMatrix23Scale(&mattmp1,sx,sy);
	BrMatrix23Mul(&mattmp2,&mattmp1,mat);
	BrMatrix23Copy(mat,&mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix23PostScale(br_matrix23 *mat, br_scalar sx, br_scalar sy)
{
        UASSERT_MESSAGE("Matrix is NULL", mat != NULL);

	BrMatrix23Scale(&mattmp1,sx,sy);
	BrMatrix23Mul(&mattmp2,mat,&mattmp1);
	BrMatrix23Copy(mat,&mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix23PreShearX(br_matrix23 *mat, br_scalar sy)
{
        UASSERT_MESSAGE("Matrix is NULL", mat != NULL);

	BrMatrix23ShearX(&mattmp1,sy);
	BrMatrix23Mul(&mattmp2,&mattmp1,mat);
	BrMatrix23Copy(mat,&mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix23PostShearX(br_matrix23 *mat, br_scalar sy)
{
        UASSERT_MESSAGE("Matrix is NULL", mat != NULL);

	BrMatrix23ShearX(&mattmp1,sy);
	BrMatrix23Mul(&mattmp2,mat,&mattmp1);
	BrMatrix23Copy(mat,&mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix23PreShearY(br_matrix23 *mat, br_scalar sx)
{
        UASSERT_MESSAGE("Matrix is NULL", mat != NULL);

	BrMatrix23ShearY(&mattmp1,sx);
	BrMatrix23Mul(&mattmp2,&mattmp1,mat);
	BrMatrix23Copy(mat,&mattmp2);
}

void BR_PUBLIC_ENTRY BrMatrix23PostShearY(br_matrix23 *mat, br_scalar sx)
{
        UASSERT_MESSAGE("Matrix is NULL", mat != NULL);

	BrMatrix23ShearY(&mattmp1,sx);
	BrMatrix23Mul(&mattmp2,mat,&mattmp1);
	BrMatrix23Copy(mat,&mattmp2);
}
