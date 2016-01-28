#include <Lgm_CTrans.h>

#include <Lgm_Vec.h>

int main( ) {

    Lgm_CTrans  *c = Lgm_init_ctrans( 1 ); 
    Lgm_Vector  Ugeo, Ugsm, Usm, Ugse1, Ugse2, tmp, Uj2000, Ugse2000, Umod;
    long int    Date;
    double      UTC, Lat, Lon, r;
    
    Date = 20130322;    // Jan 1, 2000
    //Date = 20000101;    // Jan 1, 2000
    UTC  = 5.0 + 24.0/60.0 + 24.0/3600.0;         // Universal Time Coordinated (in decimal hours)
UTC = 23.33;
//    Ugsm.x = -6.6; Ugsm.y = 3.4; Ugsm.z = -2.3; // Set a vector in GSM coordinates

    double GLON;
    GLON = -49.2;
    GLON = -165.9;
    GLON = 103.1;
    GLON = 70.0;
    GLON = -143.1;
    GLON = 14.6;
    GLON = -49.2;
    Ugeo.x = 6.619*cos( 360.0-GLON*RadPerDeg );
    Ugeo.y = 6.619*sin( 360.0-GLON*RadPerDeg );
    Ugeo.z = 0.0;
    printf("Usm  = %.8lf %.8lf %.8lf Re\n", Usm.x, Usm.y, Usm.z);



    // Set up all the necessary variables to do transformations for this Date and UTC
    /* Options for setting Sun/Moon pos are:  */
    //Lgm_Set_CTrans_Options(LGM_EPH_HIGH_ACCURACY, LGM_PN_IAU76, c); /* Uses LGM high accuracy analytic solution */
    Lgm_Set_CTrans_Options(LGM_EPH_DE, LGM_PN_IAU76, c); /* Uses JPL Development Ephemeris */
    //Lgm_Set_CTrans_Options(LGM_EPH_LOW_ACCURACY, LGM_PN_IAU76, c); /* Same as NOT calling Set_CTrans_Options */
    Lgm_Set_Coord_Transforms( Date, UTC, c );

    // Do the transformation from GSM->SM
    Lgm_Convert_Coords( &Ugeo, &Usm, GEO_TO_SM, c );
    // Do the transformation from GSM->GSE
//    Lgm_Convert_Coords( &Ugeo, &Ugse1, GEO_TO_GSE, c );
//    // Do the transformation from SM->GSE
//    Lgm_Convert_Coords( &Ugeo, &Ugse2, GEO_TO_GSE, c );
//    // Do the transformation from GSM->GSE2000
//    Lgm_Convert_Coords( &Ugeo, &Ugse2000, GEO_TO_GSE2000, c );
//    // Do the transformation from GSM->MOD
//    Lgm_Convert_Coords( &Ugeo, &Umod, GEO_TO_MOD, c );


    // Print out the final results
    printf("Date = %8ld\n", Date);
    printf("UTC  = %lf\n", UTC);
//    printf("Ugsm = %.8lf %.8lf %.8lf Re\n", Ugsm.x, Ugsm.y, Ugsm.z);
    printf("Usm  = %.8lf %.8lf %.8lf Re\n", Usm.x, Usm.y, Usm.z);
double mlon;
    mlon = atan2( Usm.y, Usm.x )*DegPerRad;

Lgm_Vector u, v;
double mmidlon;
u.x = -c->Sun.x; u.y = -c->Sun.y; u.z = -c->Sun.z;
Lgm_Convert_Coords( &u, &v, MOD_TO_GSM, c );
mmidlon = atan2( v.y, v.x )*DegPerRad;

double MLT;
MLT =  (mlon - mmidlon) / 15.0;
if (MLT < 0.0) MLT += 24.0;
if (MLT > 24.0) MLT -= 24.0;
printf("MLT: %g\n", MLT);

exit(0);


    printf("Umod = %.8lf %.8lf %.8lf Re\n", Umod.x, Umod.y, Umod.z);
    printf("\nGoing to GSE from SM and GSM\n");
    printf("Ugse  = %.8lf %.8lf %.8lf Re\n", Ugse1.x, Ugse1.y, Ugse1.z);
    printf("Ugse  = %.8lf %.8lf %.8lf Re\n", Ugse2.x, Ugse2.y, Ugse2.z);
    printf("They are different by\n");
    Lgm_VecSub(&tmp, &Ugse1, &Ugse2 );

    Lgm_PrintVector(&tmp);
    printf("\n");
    printf("\nGoing from GSM to GSE2000\n");
    printf("Ugse2000  = %.8lf %.8lf %.8lf Re\n", Ugse2000.x, Ugse2000.y, Ugse2000.z);
    printf("Difference between GSE and GSE2000\n");
    Lgm_VecSub(&tmp, &Ugse2, &Ugse2000 );
    Lgm_PrintVector(&tmp);
    Ugse2.x*=6371.2; Ugse2.y*=6371.2; Ugse2.z*=6371.2;
    Ugse2000.x*=6371.2; Ugse2000.y*=6371.2; Ugse2000.z*=6371.2;
    Lgm_VecSub(&tmp, &Ugse2, &Ugse2000 );
    printf("and in km:      %g %g %g %g\n", tmp.x, tmp.y, tmp.z, Lgm_Magnitude(&tmp));
    double delta=Lgm_VectorAngle( &Ugse2, &Ugse2000);
    printf("Ang. diff. between LGM GSE and GSE2000 = %g (%g)\n", delta, delta*3600.0);
    //Ugse1.x = -6.60046; Ugse1.y = 3.58585; Ugse1.z = -1.99640;
    //double delta=Lgm_VectorAngle( &Ugse2, &Ugse1);
    //printf("Ang. diff. between LGM GSE and TDAS GSE = %g (%g)\n", delta, delta*3600.0);
    //Lgm_VecSub(&tmp, &Ugse2, &Ugse1 );
    //delta = Lgm_Magnitude(&tmp);
    //printf("Abs. diff. between LGM GSE and TDAS GSE = %g Re (%g km)\n", delta, delta*6371.2);
    //Lgm_VecSub(&tmp, &Ugse2000, &Ugse1 );
    //delta = Lgm_Magnitude(&tmp);
    //printf("Abs. diff. between LGM GSE2000 and TDAS GSE = %g Re (%g km)\n", delta, delta*6371.2);


    // compute the ground track
    // Do the transformation from SM->WGS84
    Lgm_Convert_Coords( &Usm, &Uj2000, SM_TO_WGS84, c );
    printf("\nThe ground track point (geocentric):\n");
    Lgm_CartToSphCoords(&Uj2000, &Lat, &Lon, &r);
    printf("Lat:%lf Lon:%lf\n", Lat, Lon);


    Lgm_free_ctrans( c ); // free the structure

    return(0);
}

