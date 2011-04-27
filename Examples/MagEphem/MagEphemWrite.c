#include <Lgm_CTrans.h>
#include <Lgm_MagEphemInfo.h>
const char *sMonth[] = { "", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

/*
main(){

    FILE        *fp;

    fp = fopen( "puke.txt", "wb");
    WriteMagEphemHeader( fp, "mgh", "mithril" );
    WriteMagEphemData( fp );

}
*/


void WriteMagEphemHeader( FILE *fp, char *Spacecraft, int IdNumber, char *IntDesig, char *IntModel, char *ExtModel, double Kp, double Dst, Lgm_MagEphemInfo *m ){

    int         i, Year, Month, Day, HH, MM, SS;
    char        Str[80];
    long int    CreateDate;
    double      JD, UTC;
    Lgm_CTrans  *c = Lgm_init_ctrans(0);


    JD = Lgm_GetCurrentJD(c);
    CreateDate = Lgm_JD_to_Date( JD, &Year, &Month, &Day, &UTC );
    Lgm_UT_to_HMS( UTC, &HH, &MM, &SS );

    /*
     * Write Header
     */
    fprintf( fp, "# Spacecraft:  %s\n", Spacecraft );
//    fprintf( fp, "# Field Model:  %s\n", MagModel );
//    fprintf( fp, "# nAlpha:  %d; Alphas: ", m->nAlpha ); for (i=0; i<m->nAlpha; i++) fprintf(fp, " %g", m->Alpha[i]); fprintf( fp, ";   Units: Degrees\n");
    fprintf( fp, "#\n");
    fprintf( fp, "# File Contents    :  Magnetic Empherii for spacecraft trajectory.\n");
    fprintf( fp, "# File Created at  :  %02d:%02d:%02d UTC  %s %02d %4d\n", HH, MM, SS, sMonth[Month], Day, Year );
//    fprintf( fp, "# File Created by  :  %s\n", UserName );
//    fprintf( fp, "# File Created on  :  %s\n", Machine );
//    fprintf( fp, "# File Fmt Version :  %s\n", "" );
    fprintf( fp, "#\n");
    int nCol = 0;
    fprintf( fp, "# Description of Variables:\n");
    fprintf( fp, "# begin JSON {\n");
    fprintf( fp, "#\n");

    fprintf( fp, "# \"GlobalVarInfo\": {\n");
    fprintf( fp, "#        \"Spacecraft\": {  \"CommonName\": \"%s\",\n", Spacecraft );
    fprintf( fp, "#                           \"IdNumber\": \"%d\",\n",   IdNumber );
    fprintf( fp, "#                           \"IntDesig\": \"%s\" }\n",  IntDesig );
    fprintf( fp, "#\n");
    fprintf( fp, "# },\n");
    fprintf( fp, "#\n");


    if ( m->nAlpha > 0 ) {
        fprintf( fp, "#  \"Alpha\":                  { \"TITLE\": \"Pitch Angles.\",\n");
        fprintf( fp, "#                              \"LABEL\": \"Pitch Angle\",\n");
        fprintf( fp, "#                          \"DIMENSION\": [ %d ],\n", m->nAlpha );
        fprintf( fp, "#                             \"VALUES\": [ ");
        for (i=0; i<m->nAlpha-1; i++) fprintf(fp, "%g, ", m->Alpha[i] );
        fprintf(fp, "%g ],\n", m->Alpha[i] ); 

        fprintf( fp, "#                      \"ELEMENT_NAMES\": [ ");
        for (i=0; i<m->nAlpha-1; i++) fprintf(fp, "\"PA%02d\", ", i );
        fprintf(fp, "\"PA%02d.\" ],\n", i ); 

        fprintf( fp, "#                     \"ELEMENT_LABELS\": [ ");
        for (i=0; i<m->nAlpha-1; i++) fprintf(fp, "\"%g Deg.\", ", m->Alpha[i] );
        fprintf(fp, "\"%g Deg.\" ],\n", m->Alpha[i] ); 
        fprintf( fp, "#                              \"UNITS\": \"Degrees\",\n");
        fprintf( fp, "#                          \"VALID_MIN\":  0.0,\n");
        fprintf( fp, "#                          \"VALID_MAX\": 90.0,\n");
        fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
        fprintf( fp, "#\n");
    }
    fprintf( fp, "#\n");


    fprintf( fp, "#  \"DateTime\":               { \"TITLE\": \"The date and time in ISO 8601 compliant format.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Time\",\n");
    fprintf( fp, "#                              \"UNITS\": \"UTC\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d },\n", nCol++);
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Date\":                   { \"TITLE\": \"The date. In YYYMMDD format.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Date\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d },\n", nCol++);
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"DOY\":                    { \"TITLE\": \"Ordinal Day of Year.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Day Of Year\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                              \"UNITS\": \"Days\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": 0,\n");
    fprintf( fp, "#                          \"VALID_MAX\": 366 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"UTC\":                    { \"TITLE\": \"Universal Time (Coordinated). In decimal hours.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"UTC\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                              \"UNITS\": \"Hours\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": 0.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\": 24.0 },\n");
    fprintf( fp, "#\n");




    fprintf( fp, "#  \"JulianDate\":             { \"TITLE\": \"Julian Date. In decimal days.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Julian Date\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                              \"UNITS\": \"Days\" },\n");
    fprintf( fp, "#\n");


    fprintf( fp, "#  \"GpsTime\":                { \"TITLE\": \"Number of SI seconds since 0h Jan 6, 1980.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Gps Time\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                              \"UNITS\": \"Seconds\" },\n");
    fprintf( fp, "#\n");

    fprintf( fp, "#  \"Rgeo\":                   { \"TITLE\":  \"Geocentric Geographic position vector of S/C.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Rgeo\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 3 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 3;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [\"Rx_geo\", \"Ry_geo\", \"Rz_geo\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"Re\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -1000.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  1000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");

    fprintf( fp, "#  \"Rgeod\":                  { \"TITLE\":  \"Geodetic Geographic position vector of S/C (in Geodetic coords - (Latitude, Longitude, Height)).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Rgeod\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 3 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 3;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Rgeod_lat\", \"Rgeod_long\", \"Rgeod_height\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"Degrees\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -2.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  2.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Rgsm\":                  { \"TITLE\":  \"Geocentric Solar Magnetospheric position vector of S/C.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Rgsm\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 3 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 3;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [\"Rx_gsm\", \"Ry_gsm\", \"Rz_gsm\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"Re\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -1000.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  1000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Rsm\":                  { \"TITLE\":  \"Geocentric Solar Magnetic position vector of S/C.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Rsm\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 3 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 3;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [\"Rx_sm\", \"Ry_sm\", \"Rz_sm\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"Re\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -1000.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  1000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Rgei\":                  { \"TITLE\":  \"Geocentric Equatorial Inertial position vector of S/C.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Rgei\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 3 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 3;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [\"Rx_gei\", \"Ry_gei\", \"Rz_gei\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"Re\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -1000.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  1000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Rgse\":                  { \"TITLE\":  \"Geocentric Solar Ecliptic position vector of S/C.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Rgse\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 3 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 3;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [\"Rx_gse\", \"Ry_gse\", \"Rz_gse\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"Re\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -1000.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  1000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");


    fprintf( fp, "#  \"IntModel\":               { \"TITLE\":  \"Internal magnetic field model.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Internal Magnetic Field Model\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                               \"ENUM\": [ \"CDIP\", \"EDIP\", \"IGRF\" ]  },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"ExtModel\":               { \"TITLE\":  \"External magnetic field model.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"External Magnetic Field Model\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                               \"ENUM\": [ \"OP77\", \"T87\", \"T89\" ]  },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Kp\":                     { \"TITLE\":  \"Kp index value.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Kp\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                          \"VALID_MIN\": 0.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\": 9.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Dst\":                    { \"TITLE\":  \"Dst index value.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Dst\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                              \"UNITS\": \"nT\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -10000.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  10000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");

    fprintf( fp, "#  \"Bsc_gsm\":                { \"TITLE\":  \"Magnetic field vector at S/C (in GSM coords).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Bsc_gsm\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 4 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 4;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Bsc_gsm_x\", \"Bsc_gsm_y\", \"Bsc_gsm_z\", \"|Bsc_gsm|\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"nT\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -50000.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  50000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");

    fprintf( fp, "#  \"FieldLineType\":          { \"TITLE\":  \"Description of the type of field line the S/C is on.,\n");
    fprintf( fp, "#                                         Can be one of 4 types:\n");
    fprintf( fp, "#                                             LGM_CLOSED      - FL hits Earth at both ends.\n");
    fprintf( fp, "#                                             LGM_OPEN_N_LOBE - FL is an OPEN field line rooted in the Northern polar cap.\n");
    fprintf( fp, "#                                             LGM_OPEN_S_LOBE - FL is an OPEN field line rooted in the Southern polar cap.\n");
    fprintf( fp, "#                                             LGM_OPEN_IMF    - FL does not hit Earth at eitrher end.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Field Line Type\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                               \"ENUM\": [ \"LGM_CLOSED\", \"LGM_OPEN_N_LOBE\", \"LGM_OPEN_S_LOBE\", \"LGM_OPEN_IMF\" ]  },\n");
    fprintf( fp, "#\n");

    fprintf( fp, "#  \"Pfn_geo\":                { \"TITLE\":  \"Location of Northern Footpoint (in GEO coords).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Pfn_geo\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 3 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 3;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [\"Pfn_geo_x\", \"Pfn_geo_y\", \"Pfn_geo_z\"],\n");
    fprintf( fp, "#                              \"UNITS\": \"Re\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -2.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  2.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");

























    fprintf( fp, "#  \"Pfn_gsm\":                { \"TITLE\":  \"Location of Northern Footpoint (in GSM coords).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Pfn_gsm\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 3 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 3;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Pfs_gsm_x\", \"Pfs_gsm_y\", \"Pfs_gsm_z\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"Re\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -2.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  2.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Pfn_geod\":                { \"TITLE\":  \"Location of Northern Footpoint (in Geodetic coords - (Latitude, Longitude)).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Pfn_geod\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 2 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 2;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Pfn_geod_lat\", \"Pfn_geod_lon\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"Degrees\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -360.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  360.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Pfn_geod_height\":        { \"TITLE\":  \"Location of Northern Footpoint (in Geodetic coords - (Latitude, Longitude, Height)).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Pfn_geod_height\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                              \"UNITS\": \"km\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": 0.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\": 1000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Bfn_geo\":                { \"TITLE\":  \"Magnetic field vector at Northern Footpoint (in GEO coords).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Bfn_geo\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 4 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 4;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Bfn_geo_x\", \"Bfn_geo_y\", \"Bfn_geo_z\", \"|Bfn_geo|\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"nT\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -50000.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  50000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Bfn_gsm\":                { \"TITLE\":  \"Magnetic field vector at Northern Footpoint (in GSM coords).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Bfn_gsm\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 4 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 4;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Bfn_gsm_x\", \"Bfn_gsm_y\", \"Bfn_gsm_z\", \"|Bfn_gsm|\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"nT\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -50000.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  50000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Loss_Cone_Alpha_n\":      { \"TITLE\":  \"Value of Northern Loss Cone angle. asin( sqrt(Bsc/Bfn) ).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Loss_Cone_Alpha_n\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                              \"UNITS\": \"Degrees\",\n");
    fprintf( fp, "#                          \"VALID_MIN\":  0.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\": 90.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");











    fprintf( fp, "#  \"Pfs_geo\":                { \"TITLE\":  \"Location of Southern Footpoint (in GEO coords).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Pfs_geo\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 3 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 3;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Pfs_geo_x\", \"Pfs_geo_y\", \"Pfs_geo_z\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"Re\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -2.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  2.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Pfs_gsm\":                { \"TITLE\":  \"Location of Southern Footpoint (in GSM coords).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Pfs_gsm\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 3 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 3;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Pfs_gsm_x\", \"Pfs_gsm_y\", \"Pfs_gsm_z\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"Re\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -2.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  2.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Pfs_geod\":               { \"TITLE\":  \"Location of Southern Footpoint (in Geodetic coords - (Latitude, Longitude)).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Pfs_geod\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 2 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 2;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Pfs_geod_lat\", \"Pfs_geod_long\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"Degrees\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -360.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  360.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Pfs_geod_height\":        { \"TITLE\":  \"Location of Southern Footpoint (in Geodetic coords - (Height)).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Pfs_geod_height\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                              \"UNITS\": \"km\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": 0.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\": 1000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Bfs_geo\":                { \"TITLE\":  \"Magnetic field vector at Southern Footpoint (in GEO coords).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Bfs_geo\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 4 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 4;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Bfs_geo_x\", \"Bfs_geo_y\", \"Bfs_geo_z\", \"|Bfs_geo|\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"nT\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -60000.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  60000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Bfs_gsm\":                { \"TITLE\":  \"Magnetic field vector at Southern Footpoint (in GSM coords).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Bfs_gsm\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 4 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 4;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Bfs_gsm_x\", \"Bfs_gsm_y\", \"Bfs_gsm_z\", \"|Bfs_gsm|\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"nT\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -60000.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  60000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"Loss_Cone_Alpha_s\":      { \"TITLE\":  \"Value of Southern Loss Cone angle. asin( sqrt(Bsc/Bfs) ).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Loss_Cone_Alpha_s\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                              \"UNITS\": \"Degrees\",\n");
    fprintf( fp, "#                          \"VALID_MIN\":  0.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\": 90.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");















    fprintf( fp, "#  \"Pmin_gsm\":               { \"TITLE\":  \"Location of minimum-|B| point (in GSM coords).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Pmin_gsm\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 3 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 3;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Pmin_gsm_x\", \"Pmin_gsm_y\", \"Pmin_gsm_z\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"Re\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -200.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  200.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");

    fprintf( fp, "#  \"Bmin_gsm\":               { \"TITLE\":  \"B-field at minimum-|B| point (in GSM coords).\",\n");
    fprintf( fp, "#                              \"LABEL\": \"Bmin_gsm\",\n");
    fprintf( fp, "#                          \"DIMENSION\": [ 4 ],\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += 4;
    fprintf( fp, "#                      \"ELEMENT_NAMES\": [ \"Bmin_gsm_x\", \"Bmin_gsm_y\", \"Bmin_gsm_z\", \"|Bmin_gsm|\" ],\n");
    fprintf( fp, "#                              \"UNITS\": \"nT\",\n");
    fprintf( fp, "#                          \"VALID_MIN\": -50000.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\":  50000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");


    fprintf( fp, "#  \"M_used\":                 { \"TITLE\":  \"The magnetic dipole moment that was used to convert magnetic flux to L*. In units of nT.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"M_used\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                              \"UNITS\": \"nT\",\n");
    fprintf( fp, "#                          \"VALID_MIN\":     0.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\": 50000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"M_ref\":                  { \"TITLE\":  \"The fixed reference magnetic dipole moment for converting magnetic flux to L*. In units of nT.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"M_used\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                              \"UNITS\": \"nT\",\n");
    fprintf( fp, "#                          \"VALID_MIN\":     0.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\": 50000.0,\n");
    fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#  \"M_igrf\":                 { \"TITLE\":  \"Time-dependant magnetic dipole moment (probably shouldn't be used for converting magnetic flux to L*, but it sometimes is). In units of nT.\",\n");
    fprintf( fp, "#                              \"LABEL\": \"M_used\",\n");
    fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol++);
    fprintf( fp, "#                              \"UNITS\": \"nT\",\n");
    fprintf( fp, "#                          \"VALID_MIN\":     0.0,\n");
    fprintf( fp, "#                          \"VALID_MAX\": 50000.0,\n");
    if ( m->nAlpha > 0 ){
        fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
    } else {
        fprintf( fp, "#                         \"FILL_VALUE\": -1e31 }\n");
    }
    fprintf( fp, "#\n");





    if ( m->nAlpha > 0 ) {
        fprintf( fp, "#  \"L*\":                     { \"TITLE\": \"Generalized Roederer L-shell value.\",\n");
        fprintf( fp, "#                              \"LABEL\": \"L*\",\n");
        fprintf( fp, "#                          \"DIMENSION\": [ %d ],\n", m->nAlpha );
        fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += m->nAlpha;
        fprintf( fp, "#                      \"ELEMENT_NAMES\": [ ");
        for (i=0; i<m->nAlpha-1; i++) fprintf(fp, "\"L*(%g)\", ", m->Alpha[i] );
        fprintf(fp, "\"L*(%g)\" ],\n", m->Alpha[i] ); 
        fprintf( fp, "#                           \"DEPEND_1\": \"Alpha\",\n");
        fprintf( fp, "#                              \"UNITS\": \"Dimensionless\",\n");
        fprintf( fp, "#                          \"VALID_MIN\": \"1.0\",\n");
        fprintf( fp, "#                          \"VALID_MAX\": \"1000.0\",\n");
        fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
        fprintf( fp, "#\n");

        fprintf( fp, "#  \"L\":                      { \"TITLE\": \"McIlwain L-shell value.\",\n");
        fprintf( fp, "#                              \"LABEL\": \"L\",\n");
        fprintf( fp, "#                          \"DIMENSION\": [ %d ],\n", m->nAlpha );
        fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += m->nAlpha;
        fprintf( fp, "#                      \"ELEMENT_NAMES\": [ ");
        for (i=0; i<m->nAlpha-1; i++) fprintf(fp, "\"L(%g)\", ", m->Alpha[i] );
        fprintf(fp, "\"L*(%g)\" ],\n", m->Alpha[i] ); 
        fprintf( fp, "#                           \"DEPEND_1\": \"Alpha\",\n");
        fprintf( fp, "#                              \"UNITS\": \"Dimensionless\",\n");
        fprintf( fp, "#                          \"VALID_MIN\": \"1.0\",\n");
        fprintf( fp, "#                          \"VALID_MAX\": \"1000.0\",\n");
        fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
        fprintf( fp, "#\n");

        fprintf( fp, "#  \"Bm\":                     { \"TITLE\": \"Magnetic field ftrength at mirror points for each pitch angle.\",\n");
        fprintf( fp, "#                              \"LABEL\": \"Bm\",\n");
        fprintf( fp, "#                          \"DIMENSION\": [ %d ],\n", m->nAlpha );
        fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += m->nAlpha;
        fprintf( fp, "#                      \"ELEMENT_NAMES\": [ ");
        for (i=0; i<m->nAlpha-1; i++) fprintf(fp, "\"L(%g)\", ", m->Alpha[i] );
        fprintf(fp, "\"L*(%g)\" ],\n", m->Alpha[i] ); 
        fprintf( fp, "#                           \"DEPEND_1\": \"Alpha\",\n");
        fprintf( fp, "#                              \"UNITS\": \"nT\",\n");
        fprintf( fp, "#                          \"VALID_MIN\": \"0.0\",\n");
        fprintf( fp, "#                          \"VALID_MAX\": \"1000.0\",\n");
        fprintf( fp, "#                         \"FILL_VALUE\": -1e31 },\n");
        fprintf( fp, "#\n");

        fprintf( fp, "#  \"I\":                      { \"TITLE\": \"Integral invariant for each pitch angle.\",\n");
        fprintf( fp, "#                              \"LABEL\": \"I\",\n");
        fprintf( fp, "#                          \"DIMENSION\": [ %d ],\n", m->nAlpha );
        fprintf( fp, "#                       \"START_COLUMN\": %d,\n", nCol); nCol += m->nAlpha;
        fprintf( fp, "#                      \"ELEMENT_NAMES\": [ ");
        for (i=0; i<m->nAlpha-1; i++) fprintf(fp, "\"L(%g)\", ", m->Alpha[i] );
        fprintf(fp, "\"L*(%g)\" ],\n", m->Alpha[i] ); 
        fprintf( fp, "#                           \"DEPEND_1\": \"Alpha\",\n");
        fprintf( fp, "#                              \"UNITS\": \"Re\",\n");
        fprintf( fp, "#                          \"VALID_MIN\": \"1.0\",\n");
        fprintf( fp, "#                          \"VALID_MAX\": \"1000.0\",\n");
        fprintf( fp, "#                         \"FILL_VALUE\": -1e31 }\n");
        fprintf( fp, "#\n");

    }


    fprintf( fp, "# } end JSON\n");
    fprintf( fp, "#\n");
    fprintf( fp, "#\n");

    fprintf( fp, "#\n");
    fprintf( fp, "#\n");



    // column header
    fprintf( fp, "%91s",  "#  +------------------------------------ Date and Time -----------------------------------+" );
    fprintf( fp, " %41s",  " +--- Geocentric Geographic Coords --+" );
    fprintf( fp, " %41s",  " +---- Geodetic Geographic Coords ---+" );
    fprintf( fp, " %41s",  " +--------- GSM Coordinates ---------+" );
    fprintf( fp, " %41s",  " +---------- SM Coordinates ---------+" );
    fprintf( fp, " %41s",  " +------- GEI 2000 Coordinates ------+" );
    fprintf( fp, " %41s",  " +---------- GSE Coordinates --------+" );
    fprintf( fp, " %13s",  " +-Int Model-+" );
    fprintf( fp, " %13s",  " +-Ext Model-+" );
    fprintf( fp, " %6s",   " +-Kp-+" );
    fprintf( fp, " %7s",   " +-Dst-+" );
    fprintf( fp, " %51s",  " +--------- Magnetic Field at SpaceCraft ---------+" );
    fprintf( fp, " %29s",  " +----- Field Line Type ----+" );
    fprintf( fp, " %38s",  " +---- North Mag. Footpoint GSM -----+" );
    fprintf( fp, " %38s",  " +- North Mag. Footpoint Geographic -+" );
    fprintf( fp, " %38s",  " +-- North Mag. Footpoint Geodetic --+" );
    fprintf( fp, " %51s",  " +---- Mag. Field at North Mag. Footpoint GEO ----+" );
    fprintf( fp, " %51s",  " +---- Mag. Field at North Mag. Footpoint GSM ----+" );
    fprintf( fp, " %12s",  " +-N.L.Cone-+" );
    fprintf( fp, " %38s",  " +---- South Mag. Footpoint GSM -----+" );
    fprintf( fp, " %38s",  " +- South Mag. Footpoint Geographic -+" );
    fprintf( fp, " %38s",  " +-- South Mag. Footpoint Geodetic --+" );
    fprintf( fp, " %51s",  " +---- Mag. Field at South Mag. Footpoint GEO ----+" );
    fprintf( fp, " %51s",  " +---- Mag. Field at South Mag. Footpoint GSM ----+" );
    fprintf( fp, " %12s",  " +-S.L.Cone-+" );
    fprintf( fp, " %38s",  " +----- Minimum |B| Point GSM -------+" );
    fprintf( fp, " %51s",  " +---- Magnetic Field at Minimum |B| Pointint ----+" );

    fprintf(fp, "\n");


    // column header
    fprintf( fp, "# %25s", "Time" );
    fprintf( fp, " %10s", "Date" );
    fprintf( fp, " %5s",  "DOY" );
    fprintf( fp, " %13s", "UTC" );
    fprintf( fp, " %16s", "Julian Date" );
    fprintf( fp, " %15s", "GPS Time" );

    fprintf( fp, " %13s", "Xgeo" );
    fprintf( fp, " %13s", "Ygeo" );
    fprintf( fp, " %13s", "Zgeo" );

    fprintf( fp, " %13s", "GeodLat" );
    fprintf( fp, " %13s", "GeodLon" );
    fprintf( fp, " %13s", "GeodHeight" );

    fprintf( fp, " %13s", "Xgsm" );
    fprintf( fp, " %13s", "Ygsm" );
    fprintf( fp, " %13s", "Zgsm" );

    fprintf( fp, " %13s", "Xsm" );
    fprintf( fp, " %13s", "Ysm" );
    fprintf( fp, " %13s", "Zsm" );

    fprintf( fp, " %13s", "Xgei" );
    fprintf( fp, " %13s", "Ygei" );
    fprintf( fp, " %13s", "Zgei" );

    fprintf( fp, " %13s", "Xgse" );
    fprintf( fp, " %13s", "Ygse" );
    fprintf( fp, " %13s", "Zgse" );

    fprintf( fp, " %14s",  "Int Model" );
    fprintf( fp, " %14s",  "Ext Model" );
    fprintf( fp, " %7s",   "Kp" );
    fprintf( fp, " %8s",   "Dst" );

    fprintf( fp, " %12s", "Bsc_x" ); // Bsc  gsm
    fprintf( fp, " %12s", "Bsc_y" );
    fprintf( fp, " %12s", "Bsc_z" );
    fprintf( fp, " %12s", "Bsc" );

    fprintf( fp, " %29s",  "Field Line Type" );

    fprintf( fp, " %12s", "Xgsm" ); // n. foot
    fprintf( fp, " %12s", "Ygsm" );
    fprintf( fp, " %12s", "Zgsm" );

    fprintf( fp, " %12s", "Xgeo" );
    fprintf( fp, " %12s", "Ygeo" );
    fprintf( fp, " %12s", "Zgeo" );

    fprintf( fp, " %12s", "GeodLat" );
    fprintf( fp, " %12s", "GeodLon" );
    fprintf( fp, " %12s", "GeodHeight" );

    fprintf( fp, " %12s", "Bfn_geo_x" ); // Bfn  geo
    fprintf( fp, " %12s", "Bfn_geo_y" );
    fprintf( fp, " %12s", "Bfn_geo_z" );
    fprintf( fp, " %12s", "Bfn_geo" );

    fprintf( fp, " %12s", "Bfn_x" ); // Bfn  gsm
    fprintf( fp, " %12s", "Bfn_y" );
    fprintf( fp, " %12s", "Bfn_z" );
    fprintf( fp, " %12s", "Bfn" );

    fprintf( fp, " %12s", "Alpha_LC_N" ); // Northern Loss Cone

    fprintf( fp, " %12s", "Xgsm" ); // s. foot
    fprintf( fp, " %12s", "Ygsm" );
    fprintf( fp, " %12s", "Zgsm" );

    fprintf( fp, " %12s", "Xgeo" );
    fprintf( fp, " %12s", "Ygeo" );
    fprintf( fp, " %12s", "Zgeo" );

    fprintf( fp, " %12s", "GeodLat" );
    fprintf( fp, " %12s", "GeodLon" );
    fprintf( fp, " %12s", "GeodHeight" );

    fprintf( fp, " %12s", "Bfs_geo_x" ); // Bfs  geo
    fprintf( fp, " %12s", "Bfs_geo_y" );
    fprintf( fp, " %12s", "Bfs_geo_z" );
    fprintf( fp, " %12s", "Bfs_geo" );

    fprintf( fp, " %12s", "Bfs_x" ); // Bfs  gsm
    fprintf( fp, " %12s", "Bfs_y" );
    fprintf( fp, " %12s", "Bfs_z" );
    fprintf( fp, " %12s", "Bfs" );

    fprintf( fp, " %12s", "Alpha_LC_N" ); // Southern Loss Cone

    fprintf( fp, " %12s", "Xgsm" ); // Pmin gsm
    fprintf( fp, " %12s", "Ygsm" );
    fprintf( fp, " %12s", "Zgsm" );

    fprintf( fp, " %12s", "Bmin_x" ); // |B|min gsm
    fprintf( fp, " %12s", "Bmin_y" );
    fprintf( fp, " %12s", "Bmin_z" );
    fprintf( fp, " %12s", "Bmin" );

    fprintf( fp, " %12s", "M_used" );
    fprintf( fp, " %12s", "M_ref" );
    fprintf( fp, " %12s", "M_igrf" );
    fprintf(fp, "    ");
    for (i=0; i<m->nAlpha; i++) { sprintf( Str, "L*%d", i ); fprintf(fp, " %12s", Str ); }
    fprintf(fp, "    ");
    for (i=0; i<m->nAlpha; i++) { sprintf( Str, "L%d", i ); fprintf(fp, " %12s", Str ); }
    fprintf(fp, "    ");
    for (i=0; i<m->nAlpha; i++) { sprintf( Str, "Bm%d", i ); fprintf(fp, " %12s", Str ); }
    fprintf(fp, "    ");
    for (i=0; i<m->nAlpha; i++) { sprintf( Str, "I%d", i ); fprintf(fp, " %12s", Str ); }
    fprintf(fp, "\n");
    // units/format
    fprintf( fp, "# %25s", "YYYY-MM-DDTHH:MM:SS.SSSSZ" );
    fprintf( fp, " %10s", "YYYYMMDD" );
    fprintf( fp, " %5s",  "DDD" );
    fprintf( fp, " %13s", "Hours" );
    fprintf( fp, " %16s", "Days" );
    fprintf( fp, " %15s", "Seconds" );

    fprintf( fp, " %13s", "Re" ); // Geocentric GEO
    fprintf( fp, " %13s", "Re" );
    fprintf( fp, " %13s", "Re" );

    fprintf( fp, " %13s", "Deg." ); // Geodetic GEO
    fprintf( fp, " %13s", "Deg." );
    fprintf( fp, " %13s", "km" );

    fprintf( fp, " %13s", "Re" );
    fprintf( fp, " %13s", "Re" );
    fprintf( fp, " %13s", "Re" );

    fprintf( fp, " %13s", "Re" );
    fprintf( fp, " %13s", "Re" );
    fprintf( fp, " %13s", "Re" );

    fprintf( fp, " %13s", "Re" );
    fprintf( fp, " %13s", "Re" );
    fprintf( fp, " %13s", "Re" );

    fprintf( fp, " %13s", "Re" );
    fprintf( fp, " %13s", "Re" );
    fprintf( fp, " %13s", "Re" );

    fprintf( fp, " %14s",  " " );  // Int Model
    fprintf( fp, " %14s",  " " );  // Ext Model
    fprintf( fp, " %7s",   " " );  // Kp
    fprintf( fp, " %8s",   "nT" ); // Dst

    fprintf( fp, " %12s", "nT" );   // Bsc
    fprintf( fp, " %12s", "nT" );
    fprintf( fp, " %12s", "nT" );
    fprintf( fp, " %12s", "nT" );

    fprintf( fp, " %29s",  "" );    // Field Line Type

    fprintf( fp, " %12s", "Re" );   // GSM
    fprintf( fp, " %12s", "Re" );
    fprintf( fp, " %12s", "Re" );

    fprintf( fp, " %12s", "Re" );   // GEO
    fprintf( fp, " %12s", "Re" );
    fprintf( fp, " %12s", "Re" );

    fprintf( fp, " %12s", "Deg." ); // Geodetic
    fprintf( fp, " %12s", "Deg." );
    fprintf( fp, " %12s", "km" );

    fprintf( fp, " %12s", "nT" );   // Bfn geo
    fprintf( fp, " %12s", "nT" );
    fprintf( fp, " %12s", "nT" );
    fprintf( fp, " %12s", "nT" );

    fprintf( fp, " %12s", "nT" );   // Bfn gsm
    fprintf( fp, " %12s", "nT" );
    fprintf( fp, " %12s", "nT" );
    fprintf( fp, " %12s", "nT" );

    fprintf( fp, " %12s", "Deg." ); // Loss Cone North

    fprintf( fp, " %12s", "Re" );   // GSM
    fprintf( fp, " %12s", "Re" );
    fprintf( fp, " %12s", "Re" );

    fprintf( fp, " %12s", "Re" );   // GEO
    fprintf( fp, " %12s", "Re" );
    fprintf( fp, " %12s", "Re" );

    fprintf( fp, " %12s", "Deg." ); // Geodetic
    fprintf( fp, " %12s", "Deg." );
    fprintf( fp, " %12s", "km" );

    fprintf( fp, " %12s", "nT" );   // Bfs geo
    fprintf( fp, " %12s", "nT" );
    fprintf( fp, " %12s", "nT" );
    fprintf( fp, " %12s", "nT" );

    fprintf( fp, " %12s", "nT" );   // Bfs gsm
    fprintf( fp, " %12s", "nT" );
    fprintf( fp, " %12s", "nT" );
    fprintf( fp, " %12s", "nT" );

    fprintf( fp, " %12s", "Deg." ); // Loss Cone South

    fprintf( fp, " %12s", "Re" );   // Pmin
    fprintf( fp, " %12s", "Re" );
    fprintf( fp, " %12s", "Re" );

    fprintf( fp, " %12s", "nT" );   // Bmin
    fprintf( fp, " %12s", "nT" );
    fprintf( fp, " %12s", "nT" );
    fprintf( fp, " %12s", "nT" );

    fprintf( fp, " %12s", "nT" );   // M_Used
    fprintf( fp, " %12s", "nT" );   // M_Ref
    fprintf( fp, " %12s", "nT" );   // M_IGRF

    fprintf(fp, "    ");
    for (i=0; i<m->nAlpha; i++) { sprintf( Str, "Dimless" ); fprintf(fp, " %12s", Str ); }
    fprintf(fp, "    ");
    for (i=0; i<m->nAlpha; i++) { sprintf( Str, "Dimless" ); fprintf(fp, " %12s", Str ); }
    fprintf(fp, "    ");
    for (i=0; i<m->nAlpha; i++) { sprintf( Str, "nT" ); fprintf(fp, " %12s", Str ); }
    fprintf(fp, "    ");
    for (i=0; i<m->nAlpha; i++) { sprintf( Str, "Re" ); fprintf(fp, " %12s", Str ); }
    fprintf(fp, "\n");

    Lgm_free_ctrans(c);

}


void WriteMagEphemData( FILE *fp, char *IntModel, char *ExtModel, double Kp, double Dst, Lgm_MagEphemInfo *m ){

    int             i;
    char            Str[128];
    double          GeodLat, GeodLong, GeodHeight, L;
    double          Bsc_mag, Bfn_mag, Bfs_mag, Bmin_mag, Alpha_Loss_Cone_n, Alpha_Loss_Cone_s;
    Lgm_DateTime    DT_UTC;
    Lgm_CTrans      *c = Lgm_init_ctrans(0);
    Lgm_Vector      v, Bsc, Bfn, Bfn_geo, Bfs, Bfs_geo, Bmin;

    Lgm_Set_Coord_Transforms( m->Date, m->UTC, c );
    Lgm_Make_UTC( m->Date, m->UTC, &DT_UTC, c );
    Lgm_DateTimeToString( Str, DT_UTC, 0, 4 );
    

    fprintf( fp, "%25s",     Str );          // Date+Time in ISO 8601 format
    fprintf( fp, "   %10ld", c->UTC.Date );  // Date
    fprintf( fp, " %5d",     c->UTC.Doy );   // DOY
    fprintf( fp, " %13.8lf", c->UTC.Time );  // UTC
    fprintf( fp, " %16.8lf", c->UTC.JD );    // Julian Date
    fprintf( fp, " %15.3lf", Lgm_UTC_to_GpsSeconds( &c->UTC, c ) ); // GpsTime


    Lgm_Convert_Coords( &m->P, &v, GSM_TO_GEO, c );
    fprintf( fp, " %13.6lf", v.x );     // Xgeo
    fprintf( fp, " %13.6lf", v.y );     // Ygeo
    fprintf( fp, " %13.6lf", v.z );     // Zgeo

    Lgm_WGS84_to_GEOD( &v, &GeodLat, &GeodLong, &GeodHeight );
    fprintf( fp, " %13.6lf", GeodLat );                // Geod Lat   of SC
    fprintf( fp, " %13.6lf", GeodLong );               // Geod Long
    fprintf( fp, " %13.4lf", GeodHeight );             // Geod Height

    fprintf( fp, " %13.6lf", m->P.x );  // Xgsm
    fprintf( fp, " %13.6lf", m->P.y );  // Ygsm
    fprintf( fp, " %13.6lf", m->P.z );  // Zgsm

    Lgm_Convert_Coords( &m->P, &v, GSM_TO_SM, c );
    fprintf( fp, " %13.6lf", v.x );        // Xsm
    fprintf( fp, " %13.6lf", v.y );        // Ysm
    fprintf( fp, " %13.6lf", v.z );        // Zsm

    Lgm_Convert_Coords( &m->P, &v, GSM_TO_GEI2000, c );
    fprintf( fp, " %13.6lf", v.x );        // Xgei
    fprintf( fp, " %13.6lf", v.y );        // Ygei
    fprintf( fp, " %13.6lf", v.z );        // Zgei

    Lgm_Convert_Coords( &m->P, &v, GSM_TO_GSE, c );
    fprintf( fp, " %13.6lf", v.x );        // Xgse
    fprintf( fp, " %13.6lf", v.y );        // Ygse
    fprintf( fp, " %13.6lf", v.z );        // Zgse

    if ( !strcmp( ExtModel, "IGRF" ) || !strcmp( ExtModel, "CDIP" ) || !strcmp( ExtModel, "EDIP" ) ) {
        // If our "external model is just a dipole or igrf, then "internal doesnt really mean anything...)
        fprintf( fp, " %14s", "N/A" );       // Int model is Not applicable
    } else {
        fprintf( fp, " %14s", IntModel );    // Int Model
    }
    fprintf( fp, " %14s", ExtModel );        // Ext Model
    fprintf( fp, " %7.1f",  Kp );            // Kp
    fprintf( fp, " %8d",  (int)Dst );             // Dst
    
    m->LstarInfo->mInfo->Bfield( &m->P, &Bsc, m->LstarInfo->mInfo );
    fprintf( fp, " %12g", Bsc.x );  // Bsc_x_gsm
    fprintf( fp, " %12g", Bsc.y );  // Bsc_y_gsm
    fprintf( fp, " %12g", Bsc.z );  // Bsc_z_gsm
    fprintf( fp, " %12g", (Bsc_mag = Lgm_Magnitude( &Bsc )) );    // |B|

    switch ( m->FieldLineType ) {
        case LGM_OPEN_IMF:
                            fprintf( fp, " %29s",  "LGM_OPEN_IMF" ); // FL Type
                            break;
        case LGM_CLOSED:
                            fprintf( fp, " %29s",  "LGM_CLOSED" ); // FL Type
                            break;
        case LGM_OPEN_N_LOBE:
                            fprintf( fp, " %29s",  "LGM_OPEN_N_LOBE" ); // FL Type
                            break;
        case LGM_OPEN_S_LOBE:
                            fprintf( fp, " %29s",  "LGM_OPEN_S_LOBE" ); // FL Type
                            break;
        case LGM_INSIDE_EARTH:
                            fprintf( fp, " %29s",  "LGM_INSIDE_EARTH" ); // FL Type
                            break;
        case LGM_TARGET_HEIGHT_UNREACHABLE:
                            fprintf( fp, " %29s",  "LGM_TARGET_HEIGHT_UNREACHABLE" ); // FL Type
                            break;
        default:
                            fprintf( fp, " %29s",  "UNKNOWN FIELD TYPE" ); // FL Type
                            break;
    }

    if ( (m->FieldLineType == LGM_CLOSED) || (m->FieldLineType == LGM_OPEN_N_LOBE) ) {

        fprintf( fp, " %12g", m->Ellipsoid_Footprint_Pn.x );     // Xgsm   North Foot
        fprintf( fp, " %12g", m->Ellipsoid_Footprint_Pn.y );     // Ygsm
        fprintf( fp, " %12g", m->Ellipsoid_Footprint_Pn.z );     // Zgsm

        Lgm_Convert_Coords( &m->Ellipsoid_Footprint_Pn, &v, GSM_TO_GEO, c );
        fprintf( fp, " %12g", v.x );                    // Xgeo   North Foot
        fprintf( fp, " %12g", v.y );                    // Ygeo
        fprintf( fp, " %12g", v.z );                    // Zgeo

        Lgm_WGS84_to_GEOD( &v, &GeodLat, &GeodLong, &GeodHeight );
        fprintf( fp, " %12g", GeodLat );                // Geod Lat   North Foot
        fprintf( fp, " %12g", GeodLong );               // Geod Long
        fprintf( fp, " %12g", GeodHeight );             // Geod Height

        m->LstarInfo->mInfo->Bfield( &m->Ellipsoid_Footprint_Pn, &Bfn, m->LstarInfo->mInfo );
        Lgm_Convert_Coords( &Bfn, &Bfn_geo, GSM_TO_WGS84, c );
        fprintf( fp, " %12g", Bfn_geo.x );                                  // Bfn_x_geo
        fprintf( fp, " %12g", Bfn_geo.y );                                  // Bfn_y_geo
        fprintf( fp, " %12g", Bfn_geo.z );                                  // Bfn_z_geo
        fprintf( fp, " %12g", (Bfn_mag = Lgm_Magnitude( &Bfn_geo )) );      // |B|

        fprintf( fp, " %12g", Bfn.x );                                  // Bfn_x_gsm
        fprintf( fp, " %12g", Bfn.y );                                  // Bfn_y_gsm
        fprintf( fp, " %12g", Bfn.z );                                  // Bfn_z_gsm
        fprintf( fp, " %12g", (Bfn_mag = Lgm_Magnitude( &Bfn )) );      // |B|

        Alpha_Loss_Cone_n = asin( sqrt( Bsc_mag/Bfn_mag ) )*DegPerRad;
        fprintf( fp, " %12g", Alpha_Loss_Cone_n );                      // Northern Loss Cone Angle


    } else {

        fprintf( fp, " %12g", LGM_FILL_VALUE ); 
        fprintf( fp, " %12g", LGM_FILL_VALUE ); 
        fprintf( fp, " %12g", LGM_FILL_VALUE );

        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );

        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );

        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );

        fprintf( fp, " %12g", LGM_FILL_VALUE );

    }


    if ( (m->FieldLineType == LGM_CLOSED) || (m->FieldLineType == LGM_OPEN_S_LOBE) ) {

        fprintf( fp, " %12g", m->Ellipsoid_Footprint_Ps.x );     // Xgsm   South Foot
        fprintf( fp, " %12g", m->Ellipsoid_Footprint_Ps.y );     // Ygsm
        fprintf( fp, " %12g", m->Ellipsoid_Footprint_Ps.z );     // Zgsm

        Lgm_Convert_Coords( &m->Ellipsoid_Footprint_Ps, &v, GSM_TO_GEO, c );
        fprintf( fp, " %12g", v.x );                    // Xgeo   South Foot
        fprintf( fp, " %12g", v.y );                    // Ygeo
        fprintf( fp, " %12g", v.z );                    // Zgeo


        Lgm_WGS84_to_GEOD( &v, &GeodLat, &GeodLong, &GeodHeight );
        fprintf( fp, " %12g", GeodLat );                // Geod Lat   South Foot
        fprintf( fp, " %12g", GeodLong );               // Geod Long
        fprintf( fp, " %12g", GeodHeight );             // Geod Height

        m->LstarInfo->mInfo->Bfield( &m->Ellipsoid_Footprint_Ps, &Bfs, m->LstarInfo->mInfo );
        Lgm_Convert_Coords( &Bfs, &Bfs_geo, GSM_TO_WGS84, c );
        fprintf( fp, " %12g", Bfs_geo.x );                                  // Bfs_x_geo
        fprintf( fp, " %12g", Bfs_geo.y );                                  // Bfs_y_geo
        fprintf( fp, " %12g", Bfs_geo.z );                                  // Bfs_z_geo
        fprintf( fp, " %12g", (Bfs_mag = Lgm_Magnitude( &Bfs_geo )) );      // |B|

        fprintf( fp, " %12g", Bfs.x );                                  // Bfs_x_gsm
        fprintf( fp, " %12g", Bfs.y );                                  // Bfs_y_gsm
        fprintf( fp, " %12g", Bfs.z );                                  // Bfs_z_gsm
        fprintf( fp, " %12g", (Bfs_mag = Lgm_Magnitude( &Bfs )) );      // |B|

        Alpha_Loss_Cone_s = asin( sqrt( Bsc_mag/Bfs_mag ) )*DegPerRad;
        fprintf( fp, " %12g", Alpha_Loss_Cone_s );                      // Southern Loss Cone Angle


    } else {
        fprintf( fp, " %12g", LGM_FILL_VALUE ); 
        fprintf( fp, " %12g", LGM_FILL_VALUE ); 
        fprintf( fp, " %12g", LGM_FILL_VALUE );

        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );

        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );

        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );

        fprintf( fp, " %12g", LGM_FILL_VALUE );

    }

    

    if ( m->FieldLineType == LGM_CLOSED ) {
        fprintf( fp, " %12g", m->Pmin.x );          // Xgsm  Pmin
        fprintf( fp, " %12g", m->Pmin.y );          // Ygsm
        fprintf( fp, " %12g", m->Pmin.z );          // Zgsm

        m->LstarInfo->mInfo->Bfield( &m->Pmin, &Bmin, m->LstarInfo->mInfo );
        fprintf( fp, " %12g", Bmin.x );  // Bmin_x_gsm
        fprintf( fp, " %12g", Bmin.y );  // Bmin_y_gsm
        fprintf( fp, " %12g", Bmin.z );  // Bmin_z_gsm
        fprintf( fp, " %12g", (Bmin_mag = Lgm_Magnitude( &Bmin )) );    // |B|
    } else {
        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );

        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );
        fprintf( fp, " %12g", LGM_FILL_VALUE );
    }

    
    fprintf( fp, " %12g", m->Mused );   // M_Used
    fprintf( fp, " %12g", m->Mref );    // M_Ref
    fprintf( fp, " %12g", m->Mcurr );   // M_IGRF

    // L*'s
    fprintf(fp, "    ");
    for (i=0; i<m->nAlpha; i++) { fprintf(fp, " %12g", m->Lstar[i] ); }

    // McIlwain L (computed from I, Bm, M)
    fprintf(fp, "    ");
    for (i=0; i<m->nAlpha; i++) { 
        L = ( m->I[i] > 0.0 ) ? LFromIBmM_McIlwain(m->I[i], m->Bm[i], m->Mused ) : 0.0;
        fprintf(fp, " %12g", L);
    }

    // Bms's
    fprintf(fp, "    ");
    for (i=0; i<m->nAlpha; i++) { fprintf(fp, " %12g", m->Bm[i] ); }

    // I's
    fprintf(fp, "    ");
    for (i=0; i<m->nAlpha; i++) { fprintf(fp, " %12g", m->I[i] ); }


    fprintf(fp, "\n");

    
    Lgm_free_ctrans( c );
}
