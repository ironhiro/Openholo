#pragma once

#ifndef __ophPAS_h
#define __ophPAS_h

#include "ophGen.h"


#define PI				(3.14159265358979323846f)
#define M2_PI			(PI*2.0)
#define RADIANS			(PI/180.0)			// Angle in radians	
// DEGREE*asin(mytheta)
#define DEGREE2			(180./PI)			// sin(RADIANS*DEGREE*asin(mytheta))

#define NUMTBL			1024
#define NUMTBL2			(NUMTBL-1)
#define MAX_STR_LEN 4000

#define FFT_SEGMENT_SIZE 64

struct VoxelStruct;
struct Segment;

using namespace oph;
/**
* @addtogroup pas
//@{
* @details

* @section Introduction

Phase-added Stereogram based CGH calculation

![](@ref pics/ophgen/pas/pas1.png)

Result of the Phase-added Stereogram

![](@ref pics/ophgen/pas/PAS_result.png)


* @section Reference

Masahiro Yamaguchi, Hideshi Hoshino, Toshio Honda, Nagaaki Ohyama, "Phase-added stereogram: calculation of hologram using computer graphics technique," Proc. SPIE 1914, Practical Holography VII: Imaging and Materials, (17 September 1993)

*/
//! @} pas
/**
* @ingroup PAS
* @brief Openholo PAS based Compter-generated holography.
* @author
*/
class GEN_DLL ophPAS : public ophGen
{
private:
	OphPointCloudConfig pc_config;
	OphPointCloudData pc_data;
	int n_points;
public:
	/**
	* @brief Constructor
	* @details Initialize variables.
	*/
	explicit ophPAS();
protected:
	/**
	* @brief Destructor
	*/
	virtual ~ophPAS();

public:
	

	/**
	* @brief read config files from xml
	* @param[in] fname file name of the xml files
	* @return Type: <B>bool</B>\n
	*				If the function succeeds, the return value is <B>true</B>\n
	*				If the function fails, the return value is <B>false</B>\n
	*/
	bool readConfig(const char* fname);
	/**
	* @brief override
	* @{
	* @brief Import Point Cloud Data Base File : *.ply file.
	* This Function is included memory location of Input Point Clouds.
	*/
	/**
	* @brief override
	* @param[in] _filename PointCloud(*.ply) input file path
	* @return number of Pointcloud (if it failed loading, it returned -1)
	*/
	int loadPoint(const char* _filename);
	
	//int saveAsImg(const char * fname, uint8_t bitsperpixel, void* src, int pic_width, int pic_height);	// 이미지 저장
	int save(const char * fname, uint8_t bitsperpixel, uchar* src, uint px, uint py);
	/**
	* @brief save to image
	* @param[in] fname file name of the image files to save
	*/
	void save(const char* fname);

	


	//void PASCalcuation(long voxnum, unsigned char *cghfringe, VoxelStruct* h_vox, CGHEnvironmentData* _CGHE);
	void PASCalculation(long voxnum, unsigned char * cghfringe, OphPointCloudData *data, OphPointCloudConfig& conf);
	//void PAS(long voxelnum, struct VoxelStruct *voxel, double *m_pHologram, CGHEnvironmentData* _CGHE);
	void PAS(long voxelnum, OphPointCloudData *data, double *m_pHologram, OphPointCloudConfig& conf);
	void PAS_GPU(long voxelnum, OphPointCloudData *data, double *m_pHologram, OphPointCloudConfig& conf);
	void DataInit(int segsize, int cghwidth, int cghheight, float xiinter, float etainter);
	void DataInit(OphPointCloudConfig& conf);
	void MemoryRelease(void);

	/**
	* @brief Generate a hologram, main funtion.
	* 
	*/
	void generateHologram();
	
	void CalcSpatialFrequency(float cx, float cy, float cz, float amp, int segnumx, int segnumy, int segsize, int hsegsize, float sf_base, float * xc, float * yc, float * sf_cx, float * sf_cy, int * pp_cx, int * pp_cy, int * cf_cx, int * cf_cy, float xiint, float etaint, OphPointCloudConfig& conf);
	
	void CalcCompensatedPhase(float cx, float cy, float cz, float amp, int segnumx, int segnumy, int segsize, int hsegsize, float sf_base, float *xc, float *yc, int *cf_cx, int *cf_cy, float *COStbl, float *SINtbl, float **inRe, float **inIm, OphPointCloudConfig& conf);
	
	void RunFFTW(int segnumx, int segnumy, int segsize, int hsegsize, float **inRe, float **inIm, fftw_complex *in, fftw_complex *out, fftw_plan *plan, double *pHologram, OphPointCloudConfig& conf);

	/**
	* @brief encode Single-side band
	* @param Vector band limit
	* @param Vector specturm shift
	*/
	void encodeHologram(const vec2 band_limit, const vec2 spectrum_shift);
	void encoding(unsigned int ENCODE_FLAG);
	

	double *m_pHologram;

	float m_COStbl[NUMTBL];
	float m_SINtbl[NUMTBL];

	int m_segSize;
	int m_hsegSize;
	int m_dsegSize;
	int m_segNumx;
	int m_segNumy;
	int m_hsegNumx;
	int m_hsegNumy;

	float* m_inRe_h;
	float* m_inIm_h;

	float	*m_SFrequency_cx;
	float	*m_SFrequency_cy;
	int		*m_PickPoint_cx;
	int		*m_PickPoint_cy;
	int		*m_Coefficient_cx;
	int		*m_Coefficient_cy;
	float	*m_xc;
	float	*m_yc;
	unsigned char* cgh_fringe;

	float	m_sf_base;

	fftw_complex *m_in, *m_out;
	fftw_plan m_plan;

	float	**m_inRe;
	float	**m_inIm;

	float	m_cx;
	float	m_cy;
	float	m_cz;
	float	m_amp;
};

struct GEN_DLL VoxelStruct							// voxel structure - data
{
	int num;								// voxel or point number
	float x;								// x axis coordinate
	float y;								// y axis coordinate
	float z;								// z axis coordinate
	float ph;								// phase
	float r;								// amplitude in red channel
	float g;								// amplitude in green channel
	float b;								// amplitude in blue channel
};


struct GEN_DLL Segment
{
	bool	WorkingFlag;
	long	SegmentIndex;
	int		SegSize_x;
	int		SegSize_y;
	int 	hSegSize_x;		// Half size
	int 	hSegSize_y;		// Half size
	double	CenterX;
	double	CenterY;
	double	FrequencySlope;
};
#endif // !__ophPAS_h