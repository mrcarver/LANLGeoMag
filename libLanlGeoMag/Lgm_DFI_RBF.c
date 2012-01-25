/*! \file Lgm_DFI_RBF.c
 *  \brief Routines to perform Divergence-Free-Interpolation of vector field data (for example, B defined on meshes).
 *
 *
 *  These routines use Divergence-Free matrix-valued Radial Basis Functions (RBF) in order to
 *  interpolate a vector field from a set of scattered data points. A
 *  vector-valued interpolant \f$\vec{s}\f$ at position vector \f$\vec{x}\f$ is
 *  computed  as follows;
 *
 *      \f[
 *          \vec{s}(\vec{x}) = \sum_{j=1}^{N} \Phi( \| \vec{x} - \vec{x}_j \| ) \vec{c}_j
 *      \f]
 *
 *  where \f$ \Phi \f$ is a matrix-valued RBF;
 *
 *      \f[
 *          \Phi = \left( \begin{array}{ccc}
 *                          \Phi_{11} & \Phi_{12} & \Phi_{13} \\
 *                          \Phi_{21} & \Phi_{22} & \Phi_{23} \\
 *                          \Phi_{31} & \Phi_{32} & \Phi_{33}
 *                        \end{array} \right).
 *      \f]
 *
 *  and \f$\vec{c}\f$ is a vector-valued weigthing factor. In order to achieve
 *  a divergence-free interpolation, the matrix-valued RBF can be computed from
 *  the following;
 *
 *      \f[
 *          \Phi = \{\nabla\nabla^T - \nabla^2 I\}\: \psi( \| \vec{x} - \vec{x}_j \| )
 *      \f]
 *
 *  where \f$ \psi( \| \vec{x} - \vec{x}_j \| )\f$ is a scalar-valued RBF
 *  (e.g., see {\em "Lowitzsch S., 2002, PhD thesis, Texas A&M University",
 *  "Lowitzsch S.  Error estimates for matrix-valued radial basis function
 *  interpolation, J. Approx. Theory, 137 (2005) 238 – 249.", "McNally, C. P.,
 *  Divergence-free interpolation of vector fields from point values - exact
 *  \f$\nabla\cdot B = 0\f$ in numerical simulations, Monthly Notices of the
 *  Royal Astronomical Society, Volume 413, Issue 1, pp. L76-L80."}).
 *
 *  In matrix form the operator \f$\nabla\nabla^T - \nabla^2 I\f$ is;
 *
 *
 *      \f{eqnarray*}{
 *            {\nabla\nabla^T - \nabla^2 I} & = & { \left( \begin{array}{ccc}
 *                                                            \partial_x^2         & \partial_x\partial_y &  \partial_x\partial_z \\
 *                                                            \partial_y\partial_x & \partial_y^2         & \partial_y\partial_z  \\
 *                                                            \partial_z\partial_x & \partial_z\partial_y &          \partial_z^2  \end{array} \right)
 *                                                - \left( \begin{array}{ccc}
 *                                                             \partial_x^2 + \partial_y^2 + \partial_z^2 & 0 &0 \\
 *                                                             0 & \partial_x^2 + \partial_y^2 + \partial_z^2        & 0  \\
 *                                                             0 & 0  & \partial_x^2 + \partial_y^2 + \partial_z^2  \end{array} \right) } \\
 *                           &=& {\left( \begin{array}{ccc}
 *                                         -\partial_y^2 - \partial_z^2 & \partial_x\partial_y &  \partial_x\partial_z \\
 *                                         \partial_y\partial_x &  -\partial_x^2 - \partial_z^2        & \partial_y\partial_z  \\
 *                                         \partial_z\partial_x & \partial_z\partial_y &  -\partial_x^2 - \partial_y^2    \end{array} \right)}
 *      \f}
 *
 *
 *  We take the \f$\psi\f$ to be a scalar guassian RBF,
 *
 *
 *      \f[
 *          \psi(r) = e^{-\epsilon r^2}
 *      \f]
 *
 *  where \f$ r^2 = x^2 + y^2 + z^2 \f$ and \f$\epsilon > 0\f$. This gives,
 *
 *      \f[
 *          \begin{array}{rclcl}
 *          \Phi_{00} & = & -\partial_y^2 - \partial_z^2 & = & \left(4\epsilon - 4\epsilon^2 \left(y^2+z^2\right)\right) \psi\\
 *          \Phi_{01} & = & \partial_x\partial_y         & = &  4 \epsilon^2 x y\: \psi\\
 *          \Phi_{02} & = & \partial_x\partial_z         & = &  4 \epsilon^2 x z\: \psi\\
 *          \Phi_{10} & = & \partial_y\partial_x         & = &  \Phi_{01}\\
 *          \Phi_{11} & = & -\partial_x^2 - \partial_z^2 & = & \left(4\epsilon - 4\epsilon^2 \left(x^2+z^2\right)\right) \psi\\
 *          \Phi_{12} & = & \partial_y\partial_z         & = &  4 \epsilon^2 y z\: \psi\\
 *          \Phi_{20} & = & \partial_z\partial_x         & = &  \Phi_{02}\\
 *          \Phi_{21} & = & \partial_z\partial_y         & = &  \Phi_{12}\\
 *          \Phi_{22} & = & -\partial_x^2 - \partial_y^2 & = & \left(4\epsilon - 4\epsilon^2 \left(x^2+y^2\right)\right) \psi
 *          \end{array}
 *      \f]
 *
 *  To find the weighting vectors \f$\vec{c}_j\f$, we make sure that the
 *  interpolation agrees with all the data that we have. This leads to a linear
 *  set of equations to solve; 
 *
 *
 *      \f[
 *          {\bf A} {\bf c} = {\bf d}
 *      \f]
 *
 *  or;
 *
 *
 *      \f[ 
 *          \left( \begin{array}{cccc}
 *
 *                  \left( \begin{array}{ccc}
 *                      \Phi_{00} & \Phi_{01} & \Phi_{02} \\
 *                      \Phi_{10} & \Phi_{11} & \Phi_{12} \\
 *                      \Phi_{20} & \Phi_{21} & \Phi_{22} \\
 *                  \end{array} \right)_{0,0}
 *                  &
 *                  \left( \begin{array}{ccc}
 *                      \Phi_{00} & \Phi_{01} & \Phi_{02} \\
 *                      \Phi_{10} & \Phi_{11} & \Phi_{12} \\
 *                      \Phi_{20} & \Phi_{21} & \Phi_{22} \\
 *                  \end{array} \right)_{0,1}
 *                  &
 *                  \cdots
 *                  &
 *                  \left( \begin{array}{lll}
 *                      \Phi_{00} & \Phi_{01} & \Phi_{02} \\
 *                      \Phi_{10} & \Phi_{11} & \Phi_{12} \\
 *                      \Phi_{20} & \Phi_{21} & \Phi_{22} \\
 *                  \end{array} \right)_{0,n-1} \\
 *
 *                  \left( \begin{array}{ccc}
 *                      \Phi_{00} & \Phi_{01} & \Phi_{02} \\
 *                      \Phi_{10} & \Phi_{11} & \Phi_{12} \\
 *                      \Phi_{20} & \Phi_{21} & \Phi_{22} \\
 *                  \end{array} \right)_{1,0}
 *                  &
 *                  \left( \begin{array}{ccc}
 *                      \Phi_{00} & \Phi_{01} & \Phi_{02} \\
 *                      \Phi_{10} & \Phi_{11} & \Phi_{12} \\
 *                      \Phi_{20} & \Phi_{21} & \Phi_{22} \\
 *                  \end{array} \right)_{1,1}
 *                  &
 *                  \cdots
 *                  &
 *                  \left( \begin{array}{ccc}
 *                      \Phi_{00} & \Phi_{01} & \Phi_{02} \\
 *                      \Phi_{10} & \Phi_{11} & \Phi_{12} \\
 *                      \Phi_{20} & \Phi_{21} & \Phi_{22} \\
 *                  \end{array} \right)_{1,n-1} \\
 *
 *                  \vdots & \vdots & \ddots & \vdots \\
 *
 *                  \left( \begin{array}{ccc}
 *                      \Phi_{00} & \Phi_{01} & \Phi_{02} \\
 *                      \Phi_{10} & \Phi_{11} & \Phi_{12} \\
 *                      \Phi_{20} & \Phi_{21} & \Phi_{22} \\
 *                  \end{array} \right)_{n-1,0}
 *                  &
 *                  \left( \begin{array}{ccc}
 *                      \Phi_{00} & \Phi_{01} & \Phi_{02} \\
 *                      \Phi_{10} & \Phi_{11} & \Phi_{12} \\
 *                      \Phi_{20} & \Phi_{21} & \Phi_{22} \\
 *                  \end{array} \right)_{n-1,1}
 *                  &
 *                  \cdots
 *                  &
 *                  \left( \begin{array}{ccc}
 *                      \Phi_{00} & \Phi_{01} & \Phi_{02} \\
 *                      \Phi_{10} & \Phi_{11} & \Phi_{12} \\
 *                      \Phi_{20} & \Phi_{21} & \Phi_{22} \\
 *                  \end{array} \right)_{n-1,n-1} \\
 *
 *
 *           \end{array} \right)
 *          \left( \begin{array}{c}
 *                  c_{0,x} \\
 *                  c_{0,y} \\
 *                  c_{0,z} \\\noalign{\medskip}
 *                  c_{1,x} \\
 *                  c_{1,y} \\
 *                  c_{1,z} \\
 *                  \vdots  \\
 *                  c_{n-1,x} \\
 *                  c_{n-1,y} \\
 *                  c_{n-1,z} \\
 *           \end{array} \right)
 *          = 
 *          \left( \begin{array}{c}
 *                  d_{0,x} \\
 *                  d_{0,y} \\
 *                  d_{0,z} \\\noalign{\medskip}
 *                  d_{1,x} \\
 *                  d_{1,y} \\
 *                  d_{1,z} \\
 *                  \vdots  \\
 *                  d_{n-1,x} \\
 *                  d_{n-1,y} \\
 *                  d_{n-1,z} \\
 *           \end{array} \right)
 *  
 *      \f]
 *
 *  where the subscripts \f$i,j\f$ on each sub-matrix of A means ''evaluate the
 *  matrix elements at \f$(x,y,z) = (x_i-x_j, y_i-y_j, z_i-z_j)\f$''. Note that
 *  each submatrix of \f${\bf A}\f$ is symmetric, and the entire matrix should
 *  be as well because (for example);
 *
 *      \f[
 *          \Phi_{02}(x_i-x_j, y_i-y_j, z_i-z_j) = \Phi_{02}(x_j-x_i, y_j-y_i, z_j-z_i) = \Phi_{20}(x_j-x_i, y_j-y_i, z_j-z_i)
 *      \f]
 *
 *  We solve this system using Cholesky decomposition (which is faster than LU
 *  decomposition) in order to get the weigthing vectors \f$c\f$. 
 *
 *
 */

#include "Lgm/Lgm_DFI_RBF.h"



/** Given \f$\vec{v} = (x, y, z)\f$ and \f$\vec{v}_0 = (x_0, y_0, z_0)\f$ this
 *  routine computes the matrix elements \f$\Phi_{ij}(x-x_0, y-y_0, z-z_0)\f$. 
 *
 *  \param[in]        v   -   input target vector
 *  \param[in]       v0   -   input reference vector
 *  \param[in]      eps   -   smoothing factor in scalar RBF
 *  \param[in,out]  Phi   -   pointer to 3x3 matrix-value RBF (must must allocate and free)
 *
 *  \return  void
 *
 *  \author  M. G. Henderson
 *  \date    January 24, 2012
 *
 *
 */
void    Lgm_DFI_RBF_Phi( Lgm_Vector *v, Lgm_Vector *v0, double eps, double **Phi ) {

    double  psi, x, y, z, x2, y2, z2, r2, xy, yz, xz, f, g;

    x = v->x - v0->x;
    y = v->y - v0->y;
    z = v->z - v0->z;

    x2 = x*x; y2 = y*y; z2 = z*z;
    r2 = x2 + y2 + z2;
    xy = x*y; yz = y*z; xz = x*z;
    f = 4.0*eps; g = f*eps;

    psi = exp( -eps*r2 );

    Phi[0][0] = ( f - g*(y2 + z2) ) * psi;
    Phi[0][1] = g*xy*psi;
    Phi[0][2] = g*xz*psi;

    Phi[1][0] = Phi[0][1];
    Phi[1][1] = ( f - g*(x2 + z2) ) * psi;
    Phi[1][2] = g*yz*psi;

    Phi[2][0] = Phi[0][2];
    Phi[2][1] = Phi[1][2];
    Phi[2][2] = ( f - g*(x2 + y2) ) * psi;


    return;


}



/** From a vector-field dataset, compute the vector-valued weighting factors,
 *  \f$\vec{c}_j\f$. Info is returned in the rbf structure.
 *
 *
 *  \param[in]        v   -   pointer to an array of position vectors.
 *  \param[in]        B   -   pointer to array of corresponding field vectors.
 *  \param[in]        n   -   number of (v, B) pairs defined.
 *  \param[in]      eps   -   smoothing factor in scalar RBF.
 *
 *  \return  pointer to structure containing info for RBF interpolation. User
 *           is responsible for freeing with Lgm_DFI_RBF_Free().
 *
 *  \author  M. G. Henderson
 *  \date    January 24, 2012
 *
 *
 */
Lgm_DFI_RBF_Info *Lgm_DFI_RBF_Init( Lgm_Vector *v, Lgm_Vector *B, int n, double eps ) {

    int              i, j, ii, jj, p, q, n3;
    double           *d, **a, **Phi;
    Lgm_DFI_RBF_Info *rbf;

    n3 = 3*n;

    LGM_ARRAY_1D( d, n3, double ); 
    LGM_ARRAY_2D( a, n3, n3, double ); 
    LGM_ARRAY_2D( Phi, 3, 3, double ); 


    /*
     * Save info needed to do an evaluation.
     */
    rbf = ( Lgm_DFI_RBF_Info *)calloc( 1, sizeof(*rbf) );
    rbf->eps = eps;
    rbf->n   = n;
    rbf->n3  = n3;
    rbf->c   = gsl_vector_alloc( n3 );
    LGM_ARRAY_1D( rbf->v, n, Lgm_Vector);
    for ( i=0; i<n; i++ ) rbf->v[i] = v[i];
    


    /*
     * Fill d array.
     */
    for ( ii=0, i=0; i<n; i++ ) {
        d[ii++] = B[i].x;
        d[ii++] = B[i].y;
        d[ii++] = B[i].z;
    }


    /*
     *                                             [  row0  ]
     * Fill A matrix. In C, order is A[row][col] = [  row1  ]
     *                                             [  row2  ]
     */
    for ( i=0; i<n; i++ ) { // locate start row for subarray
        ii = 3*i;

        for ( j=0; j<n; j++ ) { // locate start column for subarray
            jj = 3*j;

            // Get Phi( v_i - v_j )
            Lgm_DFI_RBF_Phi( &v[i], &v[j], eps, Phi );

            for ( p=0; p<3; p++ ){ // subarray row
                for ( q=0; q<3; q++ ){  // subarray column
                    a[ii+p][jj+q] = Phi[p][q];
                }
            }


        }

    }


    /*
     * Now we need to solve the system of equation;
     *
     *      d = ac
     *
     *  for c.
     *
     *  First create gsl_vector and gsl_matrix views of the d and A arrays.
     *  Then compute Cholesky decomposition of the a array. Then solve the
     *  system to get c.
     *
     */
    gsl_vector_view D = gsl_vector_view_array( d, n3 );
    gsl_matrix_view A = gsl_matrix_view_array( a, n3, n3 );
    gsl_linalg_cholesky_decomp( &A.matrix );
    gsl_linalg_cholesky_solve( &A.matrix, &D.vector, rbf->c );


    



    LGM_ARRAY_1D_FREE( d ); 
    LGM_ARRAY_2D_FREE( a ); 
    LGM_ARRAY_2D_FREE( Phi ); 

    return( rbf );

}

/** Free a previously allocated Lgm_DFI_RBF_Info structure.
 *
 *
 *  \param[in]     rbf   -   pointer to structure containing info for RBF interpolation. 
 *
 *  \return  void
 *
 *  \author  M. G. Henderson
 *  \date    January 24, 2012
 *
 *
 */
void    Lgm_DFI_RBF_Free( Lgm_DFI_RBF_Info *rbf ) {
    LGM_ARRAY_1D_FREE( rbf->v );
    gsl_vector_free( rbf->c );
    return;
}




/**  Compute Divergence Free interpolant at the specified position vector. The
 *   weights given in \f$\vec{c}\f$ must have been pre-computed with
 *   Lgm_DFI_RBF_Init().
 *
 *
 *  \param[in]        v   -   position vector to compute B at.
 *  \param[out]       B   -   interpolated value of B at v.
 *  \param[out]     rbf   -   pointer to initialized Lgm_DFI_RBF_Info structure.
 *
 *  \return  void
 *
 *  \author  M. G. Henderson
 *  \date    January 24, 2012
 *
 *
 */
void    Lgm_DFI_RBF_Eval( Lgm_Vector *v, Lgm_Vector *B, Lgm_DFI_RBF_Info *rbf ) {

    int         j;
    Lgm_Vector  W;
    double      Phi[3][3];

    B->x = B->y = B->z = 0.0;
    for ( j=0; j<rbf->n; j++ ){

        Lgm_DFI_RBF_Phi( v, &rbf->v[j], rbf->eps, Phi );
        Lgm_MatTimesVec( Phi, &rbf->v[j], &W );
        B->x += W.x; B->y += W.y; B->z += W.z;

    }

    return;
    
}

