/* sgerq2.f -- translated by f2c (version 20061008).
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

 int sgerq2_(int *m, int *n, float *a, int *lda, 
	float *tau, float *work, int *info)
{
    /* System generated locals */
    int a_dim1, a_offset, i__1, i__2;

    /* Local variables */
    int i__, k;
    float aii;
    extern  int slarf_(char *, int *, int *, float *, 
	    int *, float *, float *, int *, float *), xerbla_(
	    char *, int *), slarfp_(int *, float *, float *, 
	    int *, float *);


/*  -- LAPACK routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  SGERQ2 computes an RQ factorization of a float m by n matrix A: */
/*  A = R * Q. */

/*  Arguments */
/*  ========= */

/*  M       (input) INTEGER */
/*          The number of rows of the matrix A.  M >= 0. */

/*  N       (input) INTEGER */
/*          The number of columns of the matrix A.  N >= 0. */

/*  A       (input/output) REAL array, dimension (LDA,N) */
/*          On entry, the m by n matrix A. */
/*          On exit, if m <= n, the upper triangle of the subarray */
/*          A(1:m,n-m+1:n) contains the m by m upper triangular matrix R; */
/*          if m >= n, the elements on and above the (m-n)-th subdiagonal */
/*          contain the m by n upper trapezoidal matrix R; the remaining */
/*          elements, with the array TAU, represent the orthogonal matrix */
/*          Q as a product of elementary reflectors (see Further */
/*          Details). */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= MAX(1,M). */

/*  TAU     (output) REAL array, dimension (MIN(M,N)) */
/*          The scalar factors of the elementary reflectors (see Further */
/*          Details). */

/*  WORK    (workspace) REAL array, dimension (M) */

/*  INFO    (output) INTEGER */
/*          = 0: successful exit */
/*          < 0: if INFO = -i, the i-th argument had an illegal value */

/*  Further Details */
/*  =============== */

/*  The matrix Q is represented as a product of elementary reflectors */

/*     Q = H(1) H(2) . . . H(k), where k = MIN(m,n). */

/*  Each H(i) has the form */

/*     H(i) = I - tau * v * v' */

/*  where tau is a float scalar, and v is a float vector with */
/*  v(n-k+i+1:n) = 0 and v(n-k+i) = 1; v(1:n-k+i-1) is stored on exit in */
/*  A(m-k+i,1:n-k+i-1), and tau in TAU(i). */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input arguments */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --tau;
    --work;

    /* Function Body */
    *info = 0;
    if (*m < 0) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*lda < MAX(1,*m)) {
	*info = -4;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("SGERQ2", &i__1);
	return 0;
    }

    k = MIN(*m,*n);

    for (i__ = k; i__ >= 1; --i__) {

/*        Generate elementary reflector H(i) to annihilate */
/*        A(m-k+i,1:n-k+i-1) */

	i__1 = *n - k + i__;
	slarfp_(&i__1, &a[*m - k + i__ + (*n - k + i__) * a_dim1], &a[*m - k 
		+ i__ + a_dim1], lda, &tau[i__]);

/*        Apply H(i) to A(1:m-k+i-1,1:n-k+i) from the right */

	aii = a[*m - k + i__ + (*n - k + i__) * a_dim1];
	a[*m - k + i__ + (*n - k + i__) * a_dim1] = 1.f;
	i__1 = *m - k + i__ - 1;
	i__2 = *n - k + i__;
	slarf_("Right", &i__1, &i__2, &a[*m - k + i__ + a_dim1], lda, &tau[
		i__], &a[a_offset], lda, &work[1]);
	a[*m - k + i__ + (*n - k + i__) * a_dim1] = aii;
/* L10: */
    }
    return 0;

/*     End of SGERQ2 */

} /* sgerq2_ */
