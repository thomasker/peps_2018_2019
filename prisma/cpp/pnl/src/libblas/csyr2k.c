/* csyr2k.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "pnl/pnl_f2c.h"

 int csyr2k_(char *uplo, char *trans, int *n, int *k, 
	complex *alpha, complex *a, int *lda, complex *b, int *ldb, 
	complex *beta, complex *c__, int *ldc)
{
    /* System generated locals */
    int a_dim1, a_offset, b_dim1, b_offset, c_dim1, c_offset, i__1, i__2, 
	    i__3, i__4, i__5, i__6, i__7;
    complex q__1, q__2, q__3, q__4, q__5;

    /* Local variables */
    int i__, j, l, info;
    complex temp1, temp2;
    extern int lsame_(char *, char *);
    int nrowa;
    int upper;
    extern  int xerbla_(char *, int *);

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CSYR2K  performs one of the symmetric rank 2k operations */

/*     C := alpha*A*B' + alpha*B*A' + beta*C, */

/*  or */

/*     C := alpha*A'*B + alpha*B'*A + beta*C, */

/*  where  alpha and beta  are scalars,  C is an  n by n symmetric matrix */
/*  and  A and B  are  n by k  matrices  in the  first  case  and  k by n */
/*  matrices in the second case. */

/*  Arguments */
/*  ========== */

/*  UPLO   - CHARACTER*1. */
/*           On  entry,   UPLO  specifies  whether  the  upper  or  lower */
/*           triangular  part  of the  array  C  is to be  referenced  as */
/*           follows: */

/*              UPLO = 'U' or 'u'   Only the  upper triangular part of  C */
/*                                  is to be referenced. */

/*              UPLO = 'L' or 'l'   Only the  lower triangular part of  C */
/*                                  is to be referenced. */

/*           Unchanged on exit. */

/*  TRANS  - CHARACTER*1. */
/*           On entry,  TRANS  specifies the operation to be performed as */
/*           follows: */

/*              TRANS = 'N' or 'n'    C := alpha*A*B' + alpha*B*A' + */
/*                                         beta*C. */

/*              TRANS = 'T' or 't'    C := alpha*A'*B + alpha*B'*A + */
/*                                         beta*C. */

/*           Unchanged on exit. */

/*  N      - INTEGER. */
/*           On entry,  N specifies the order of the matrix C.  N must be */
/*           at least zero. */
/*           Unchanged on exit. */

/*  K      - INTEGER. */
/*           On entry with  TRANS = 'N' or 'n',  K  specifies  the number */
/*           of  columns  of the  matrices  A and B,  and on  entry  with */
/*           TRANS = 'T' or 't',  K  specifies  the number of rows of the */
/*           matrices  A and B.  K must be at least zero. */
/*           Unchanged on exit. */

/*  ALPHA  - COMPLEX         . */
/*           On entry, ALPHA specifies the scalar alpha. */
/*           Unchanged on exit. */

/*  A      - COMPLEX          array of DIMENSION ( LDA, ka ), where ka is */
/*           k  when  TRANS = 'N' or 'n',  and is  n  otherwise. */
/*           Before entry with  TRANS = 'N' or 'n',  the  leading  n by k */
/*           part of the array  A  must contain the matrix  A,  otherwise */
/*           the leading  k by n  part of the array  A  must contain  the */
/*           matrix A. */
/*           Unchanged on exit. */

/*  LDA    - INTEGER. */
/*           On entry, LDA specifies the first dimension of A as declared */
/*           in  the  calling  (sub)  program.   When  TRANS = 'N' or 'n' */
/*           then  LDA must be at least  MAX( 1, n ), otherwise  LDA must */
/*           be at least  MAX( 1, k ). */
/*           Unchanged on exit. */

/*  B      - COMPLEX          array of DIMENSION ( LDB, kb ), where kb is */
/*           k  when  TRANS = 'N' or 'n',  and is  n  otherwise. */
/*           Before entry with  TRANS = 'N' or 'n',  the  leading  n by k */
/*           part of the array  B  must contain the matrix  B,  otherwise */
/*           the leading  k by n  part of the array  B  must contain  the */
/*           matrix B. */
/*           Unchanged on exit. */

/*  LDB    - INTEGER. */
/*           On entry, LDB specifies the first dimension of B as declared */
/*           in  the  calling  (sub)  program.   When  TRANS = 'N' or 'n' */
/*           then  LDB must be at least  MAX( 1, n ), otherwise  LDB must */
/*           be at least  MAX( 1, k ). */
/*           Unchanged on exit. */

/*  BETA   - COMPLEX         . */
/*           On entry, BETA specifies the scalar beta. */
/*           Unchanged on exit. */

/*  C      - COMPLEX          array of DIMENSION ( LDC, n ). */
/*           Before entry  with  UPLO = 'U' or 'u',  the leading  n by n */
/*           upper triangular part of the array C must contain the upper */
/*           triangular part  of the  symmetric matrix  and the strictly */
/*           lower triangular part of C is not referenced.  On exit, the */
/*           upper triangular part of the array  C is overwritten by the */
/*           upper triangular part of the updated matrix. */
/*           Before entry  with  UPLO = 'L' or 'l',  the leading  n by n */
/*           lower triangular part of the array C must contain the lower */
/*           triangular part  of the  symmetric matrix  and the strictly */
/*           upper triangular part of C is not referenced.  On exit, the */
/*           lower triangular part of the array  C is overwritten by the */
/*           lower triangular part of the updated matrix. */

/*  LDC    - INTEGER. */
/*           On entry, LDC specifies the first dimension of C as declared */
/*           in  the  calling  (sub)  program.   LDC  must  be  at  least */
/*           MAX( 1, n ). */
/*           Unchanged on exit. */


/*  Level 3 Blas routine. */

/*  -- Written on 8-February-1989. */
/*     Jack Dongarra, Argonne National Laboratory. */
/*     Iain Duff, AERE Harwell. */
/*     Jeremy Du Croz, Numerical Algorithms Group Ltd. */
/*     Sven Hammarling, Numerical Algorithms Group Ltd. */


/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Parameters .. */
/*     .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    c_dim1 = *ldc;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;

    /* Function Body */
    if (lsame_(trans, "N")) {
	nrowa = *n;
    } else {
	nrowa = *k;
    }
    upper = lsame_(uplo, "U");

    info = 0;
    if (! upper && ! lsame_(uplo, "L")) {
	info = 1;
    } else if (! lsame_(trans, "N") && ! lsame_(trans, 
	    "T")) {
	info = 2;
    } else if (*n < 0) {
	info = 3;
    } else if (*k < 0) {
	info = 4;
    } else if (*lda < MAX(1,nrowa)) {
	info = 7;
    } else if (*ldb < MAX(1,nrowa)) {
	info = 9;
    } else if (*ldc < MAX(1,*n)) {
	info = 12;
    }
    if (info != 0) {
	xerbla_("CSYR2K", &info);
	return 0;
    }

/*     Quick return if possible. */

    if (*n == 0 || (alpha->r == 0.f && alpha->i == 0.f || *k == 0) && (
	    beta->r == 1.f && beta->i == 0.f)) {
	return 0;
    }

/*     And when  alpha.eq.zero. */

    if (alpha->r == 0.f && alpha->i == 0.f) {
	if (upper) {
	    if (beta->r == 0.f && beta->i == 0.f) {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = j;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			i__3 = i__ + j * c_dim1;
			c__[i__3].r = 0.f, c__[i__3].i = 0.f;
/* L10: */
		    }
/* L20: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = j;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			i__3 = i__ + j * c_dim1;
			i__4 = i__ + j * c_dim1;
			q__1.r = beta->r * c__[i__4].r - beta->i * c__[i__4]
				.i, q__1.i = beta->r * c__[i__4].i + beta->i *
				 c__[i__4].r;
			c__[i__3].r = q__1.r, c__[i__3].i = q__1.i;
/* L30: */
		    }
/* L40: */
		}
	    }
	} else {
	    if (beta->r == 0.f && beta->i == 0.f) {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *n;
		    for (i__ = j; i__ <= i__2; ++i__) {
			i__3 = i__ + j * c_dim1;
			c__[i__3].r = 0.f, c__[i__3].i = 0.f;
/* L50: */
		    }
/* L60: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *n;
		    for (i__ = j; i__ <= i__2; ++i__) {
			i__3 = i__ + j * c_dim1;
			i__4 = i__ + j * c_dim1;
			q__1.r = beta->r * c__[i__4].r - beta->i * c__[i__4]
				.i, q__1.i = beta->r * c__[i__4].i + beta->i *
				 c__[i__4].r;
			c__[i__3].r = q__1.r, c__[i__3].i = q__1.i;
/* L70: */
		    }
/* L80: */
		}
	    }
	}
	return 0;
    }

/*     Start the operations. */

    if (lsame_(trans, "N")) {

/*        Form  C := alpha*A*B' + alpha*B*A' + C. */

	if (upper) {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		if (beta->r == 0.f && beta->i == 0.f) {
		    i__2 = j;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			i__3 = i__ + j * c_dim1;
			c__[i__3].r = 0.f, c__[i__3].i = 0.f;
/* L90: */
		    }
		} else if (beta->r != 1.f || beta->i != 0.f) {
		    i__2 = j;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			i__3 = i__ + j * c_dim1;
			i__4 = i__ + j * c_dim1;
			q__1.r = beta->r * c__[i__4].r - beta->i * c__[i__4]
				.i, q__1.i = beta->r * c__[i__4].i + beta->i *
				 c__[i__4].r;
			c__[i__3].r = q__1.r, c__[i__3].i = q__1.i;
/* L100: */
		    }
		}
		i__2 = *k;
		for (l = 1; l <= i__2; ++l) {
		    i__3 = j + l * a_dim1;
		    i__4 = j + l * b_dim1;
		    if (a[i__3].r != 0.f || a[i__3].i != 0.f || (b[i__4].r != 
			    0.f || b[i__4].i != 0.f)) {
			i__3 = j + l * b_dim1;
			q__1.r = alpha->r * b[i__3].r - alpha->i * b[i__3].i, 
				q__1.i = alpha->r * b[i__3].i + alpha->i * b[
				i__3].r;
			temp1.r = q__1.r, temp1.i = q__1.i;
			i__3 = j + l * a_dim1;
			q__1.r = alpha->r * a[i__3].r - alpha->i * a[i__3].i, 
				q__1.i = alpha->r * a[i__3].i + alpha->i * a[
				i__3].r;
			temp2.r = q__1.r, temp2.i = q__1.i;
			i__3 = j;
			for (i__ = 1; i__ <= i__3; ++i__) {
			    i__4 = i__ + j * c_dim1;
			    i__5 = i__ + j * c_dim1;
			    i__6 = i__ + l * a_dim1;
			    q__3.r = a[i__6].r * temp1.r - a[i__6].i * 
				    temp1.i, q__3.i = a[i__6].r * temp1.i + a[
				    i__6].i * temp1.r;
			    q__2.r = c__[i__5].r + q__3.r, q__2.i = c__[i__5]
				    .i + q__3.i;
			    i__7 = i__ + l * b_dim1;
			    q__4.r = b[i__7].r * temp2.r - b[i__7].i * 
				    temp2.i, q__4.i = b[i__7].r * temp2.i + b[
				    i__7].i * temp2.r;
			    q__1.r = q__2.r + q__4.r, q__1.i = q__2.i + 
				    q__4.i;
			    c__[i__4].r = q__1.r, c__[i__4].i = q__1.i;
/* L110: */
			}
		    }
/* L120: */
		}
/* L130: */
	    }
	} else {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		if (beta->r == 0.f && beta->i == 0.f) {
		    i__2 = *n;
		    for (i__ = j; i__ <= i__2; ++i__) {
			i__3 = i__ + j * c_dim1;
			c__[i__3].r = 0.f, c__[i__3].i = 0.f;
/* L140: */
		    }
		} else if (beta->r != 1.f || beta->i != 0.f) {
		    i__2 = *n;
		    for (i__ = j; i__ <= i__2; ++i__) {
			i__3 = i__ + j * c_dim1;
			i__4 = i__ + j * c_dim1;
			q__1.r = beta->r * c__[i__4].r - beta->i * c__[i__4]
				.i, q__1.i = beta->r * c__[i__4].i + beta->i *
				 c__[i__4].r;
			c__[i__3].r = q__1.r, c__[i__3].i = q__1.i;
/* L150: */
		    }
		}
		i__2 = *k;
		for (l = 1; l <= i__2; ++l) {
		    i__3 = j + l * a_dim1;
		    i__4 = j + l * b_dim1;
		    if (a[i__3].r != 0.f || a[i__3].i != 0.f || (b[i__4].r != 
			    0.f || b[i__4].i != 0.f)) {
			i__3 = j + l * b_dim1;
			q__1.r = alpha->r * b[i__3].r - alpha->i * b[i__3].i, 
				q__1.i = alpha->r * b[i__3].i + alpha->i * b[
				i__3].r;
			temp1.r = q__1.r, temp1.i = q__1.i;
			i__3 = j + l * a_dim1;
			q__1.r = alpha->r * a[i__3].r - alpha->i * a[i__3].i, 
				q__1.i = alpha->r * a[i__3].i + alpha->i * a[
				i__3].r;
			temp2.r = q__1.r, temp2.i = q__1.i;
			i__3 = *n;
			for (i__ = j; i__ <= i__3; ++i__) {
			    i__4 = i__ + j * c_dim1;
			    i__5 = i__ + j * c_dim1;
			    i__6 = i__ + l * a_dim1;
			    q__3.r = a[i__6].r * temp1.r - a[i__6].i * 
				    temp1.i, q__3.i = a[i__6].r * temp1.i + a[
				    i__6].i * temp1.r;
			    q__2.r = c__[i__5].r + q__3.r, q__2.i = c__[i__5]
				    .i + q__3.i;
			    i__7 = i__ + l * b_dim1;
			    q__4.r = b[i__7].r * temp2.r - b[i__7].i * 
				    temp2.i, q__4.i = b[i__7].r * temp2.i + b[
				    i__7].i * temp2.r;
			    q__1.r = q__2.r + q__4.r, q__1.i = q__2.i + 
				    q__4.i;
			    c__[i__4].r = q__1.r, c__[i__4].i = q__1.i;
/* L160: */
			}
		    }
/* L170: */
		}
/* L180: */
	    }
	}
    } else {

/*        Form  C := alpha*A'*B + alpha*B'*A + C. */

	if (upper) {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		i__2 = j;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    temp1.r = 0.f, temp1.i = 0.f;
		    temp2.r = 0.f, temp2.i = 0.f;
		    i__3 = *k;
		    for (l = 1; l <= i__3; ++l) {
			i__4 = l + i__ * a_dim1;
			i__5 = l + j * b_dim1;
			q__2.r = a[i__4].r * b[i__5].r - a[i__4].i * b[i__5]
				.i, q__2.i = a[i__4].r * b[i__5].i + a[i__4]
				.i * b[i__5].r;
			q__1.r = temp1.r + q__2.r, q__1.i = temp1.i + q__2.i;
			temp1.r = q__1.r, temp1.i = q__1.i;
			i__4 = l + i__ * b_dim1;
			i__5 = l + j * a_dim1;
			q__2.r = b[i__4].r * a[i__5].r - b[i__4].i * a[i__5]
				.i, q__2.i = b[i__4].r * a[i__5].i + b[i__4]
				.i * a[i__5].r;
			q__1.r = temp2.r + q__2.r, q__1.i = temp2.i + q__2.i;
			temp2.r = q__1.r, temp2.i = q__1.i;
/* L190: */
		    }
		    if (beta->r == 0.f && beta->i == 0.f) {
			i__3 = i__ + j * c_dim1;
			q__2.r = alpha->r * temp1.r - alpha->i * temp1.i, 
				q__2.i = alpha->r * temp1.i + alpha->i * 
				temp1.r;
			q__3.r = alpha->r * temp2.r - alpha->i * temp2.i, 
				q__3.i = alpha->r * temp2.i + alpha->i * 
				temp2.r;
			q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
			c__[i__3].r = q__1.r, c__[i__3].i = q__1.i;
		    } else {
			i__3 = i__ + j * c_dim1;
			i__4 = i__ + j * c_dim1;
			q__3.r = beta->r * c__[i__4].r - beta->i * c__[i__4]
				.i, q__3.i = beta->r * c__[i__4].i + beta->i *
				 c__[i__4].r;
			q__4.r = alpha->r * temp1.r - alpha->i * temp1.i, 
				q__4.i = alpha->r * temp1.i + alpha->i * 
				temp1.r;
			q__2.r = q__3.r + q__4.r, q__2.i = q__3.i + q__4.i;
			q__5.r = alpha->r * temp2.r - alpha->i * temp2.i, 
				q__5.i = alpha->r * temp2.i + alpha->i * 
				temp2.r;
			q__1.r = q__2.r + q__5.r, q__1.i = q__2.i + q__5.i;
			c__[i__3].r = q__1.r, c__[i__3].i = q__1.i;
		    }
/* L200: */
		}
/* L210: */
	    }
	} else {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		i__2 = *n;
		for (i__ = j; i__ <= i__2; ++i__) {
		    temp1.r = 0.f, temp1.i = 0.f;
		    temp2.r = 0.f, temp2.i = 0.f;
		    i__3 = *k;
		    for (l = 1; l <= i__3; ++l) {
			i__4 = l + i__ * a_dim1;
			i__5 = l + j * b_dim1;
			q__2.r = a[i__4].r * b[i__5].r - a[i__4].i * b[i__5]
				.i, q__2.i = a[i__4].r * b[i__5].i + a[i__4]
				.i * b[i__5].r;
			q__1.r = temp1.r + q__2.r, q__1.i = temp1.i + q__2.i;
			temp1.r = q__1.r, temp1.i = q__1.i;
			i__4 = l + i__ * b_dim1;
			i__5 = l + j * a_dim1;
			q__2.r = b[i__4].r * a[i__5].r - b[i__4].i * a[i__5]
				.i, q__2.i = b[i__4].r * a[i__5].i + b[i__4]
				.i * a[i__5].r;
			q__1.r = temp2.r + q__2.r, q__1.i = temp2.i + q__2.i;
			temp2.r = q__1.r, temp2.i = q__1.i;
/* L220: */
		    }
		    if (beta->r == 0.f && beta->i == 0.f) {
			i__3 = i__ + j * c_dim1;
			q__2.r = alpha->r * temp1.r - alpha->i * temp1.i, 
				q__2.i = alpha->r * temp1.i + alpha->i * 
				temp1.r;
			q__3.r = alpha->r * temp2.r - alpha->i * temp2.i, 
				q__3.i = alpha->r * temp2.i + alpha->i * 
				temp2.r;
			q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
			c__[i__3].r = q__1.r, c__[i__3].i = q__1.i;
		    } else {
			i__3 = i__ + j * c_dim1;
			i__4 = i__ + j * c_dim1;
			q__3.r = beta->r * c__[i__4].r - beta->i * c__[i__4]
				.i, q__3.i = beta->r * c__[i__4].i + beta->i *
				 c__[i__4].r;
			q__4.r = alpha->r * temp1.r - alpha->i * temp1.i, 
				q__4.i = alpha->r * temp1.i + alpha->i * 
				temp1.r;
			q__2.r = q__3.r + q__4.r, q__2.i = q__3.i + q__4.i;
			q__5.r = alpha->r * temp2.r - alpha->i * temp2.i, 
				q__5.i = alpha->r * temp2.i + alpha->i * 
				temp2.r;
			q__1.r = q__2.r + q__5.r, q__1.i = q__2.i + q__5.i;
			c__[i__3].r = q__1.r, c__[i__3].i = q__1.i;
		    }
/* L230: */
		}
/* L240: */
	    }
	}
    }

    return 0;

/*     End of CSYR2K. */

} /* csyr2k_ */
