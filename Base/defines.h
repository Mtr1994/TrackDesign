#ifndef DEFINES_H
#define DEFINES_H

#include <vector>
#include <string>

using namespace std;

/* multiply this by degrees to get radians */
#define DTR 0.01745329251994329500

/* multiply this by radians to get degrees */
#define RTD 57.2957795130823230000

/* ellipsoid coefficients from World Geodetic System Ellipsoid of 1972
 * - see Bowditch (H.O. 9 -- American Practical Navigator). */
const double C1 = 111412.84;
const double C2 = -93.5;
const double C3 = 0.118;
const double C4 = 111132.92;
const double C5 = -559.82;
const double C6 = 1.175;
const double C7 = 0.0023;

typedef struct {
    float x;
    float y;
    float z;
    float scalar;
    bool empty;
}Point;

typedef struct {
    int xSize;
    int ySize;
    vector<Point*> points;
}BlockPoints; // 栅格数据

typedef struct {
    vector<float> x;
    vector<float> y;
    vector<float> z;
    vector<int> amp;
    int ns;
    float range;
}WcdBeamPoint;

typedef struct {
    string rawfile;
    long long datetime;
    float longtitude;
    float latitude;
    short num_datagram;
    short total_beams;
    int total_samples;
    int max_samples;
    short ss;                   //sound speed
    short sf;                   //Sampling frequence in 0.01 Hz resolution
    int ping;
    WcdBeamPoint* beam_data;
}WcdFileFanInfo;

//////////////////////// all file
typedef struct {
    double x;
    double y;
    double z;
}all_beam_point;

typedef struct {
    long long datetime;
    int longitude;
    int latitude;

    int pingcounter;
    int heading;
    int numofbeams;

    short num_datagram;
    short total_beams;
    short ss;  //sound speed
    short sf;  //Sampling frequence in 0.01 Hz resolution
    all_beam_point* beam_data;
}all_xyz88_info;

typedef struct {
    long long datetime;
    int longitude;
    int latitude;
    string posinput;
}all_position_info;

typedef struct {
    double min;
    double max;
    BlockPoints points;
}RasterFileDetails;

typedef struct {
    string rawfile;
    string position;
    long long datetime;
    long long from_time; // 开始时间
    long long end_time; // 结束时间
    int total_package; // 总包数
    int water_column_package; // 水体包数
    int ping;
    int total_beams;
    int max_samples; // 单独波束最大点数量
    vector<long long> time_list; // 时间列表
    vector<vector<Point*>> points;
}WcdFileDetails;

typedef struct {
    string file;
    double min;
    double max;
    long long from_seabed_time; // 地形开始时间
    long long end_seabed_time; // 地形结束时间
    long long from_position_time; // 位置开始时间
    long long end_position_time; // 位置结束时间
    int total_package; // 总包数
    int seabed_package; // 地形包数
    int position_package; // 位置包个数
    vector<vector<Point*>> points;
}SeabedFileDetails;

typedef struct {
    float longitude;
    float latitude;
    float x;
    float y;
    int heading;
}Position;

/////////////////////////////////////国产设备数据结构////////////////////////////////////////////////////

// Nt 发送波束数据体
typedef struct {
    unsigned int seqid;
    unsigned int date;
    unsigned int time; 					// 0.001 秒
    unsigned int anglex; 				// 0.0001 度 （0~180）度
}TransmitData;

// Nr 接收波束数据体
typedef struct {
    unsigned int seqid;
    unsigned int transmitseqid;
    unsigned int angley; 				// 0.0001 度 （0~180）度
    unsigned int spendtime;
    int amp; 							// 0.0001
    unsigned int quality; 				// 0.0001
    int longitude; 						// 0.0000001 （-90~90）
    int latitude; 						// 0.0000001 (-90~90)
    int depth; 							// 0.0001 米 （0~12000）
}ReceiveData;

// 测深结果数据包
typedef struct {
    char* sign;
    unsigned short id;
    unsigned short userid;
    unsigned int counter;
    unsigned int length;
    unsigned int ping;
    unsigned int nt;
    unsigned int nr;
    std::vector<TransmitData> vectortransmit;
    std::vector<ReceiveData> vectorreceive;
}DepthPackage;

// 惯导子体数据包
typedef struct {
    unsigned int date;
    unsigned int time;					// 0.001 秒
    unsigned short heading; 			// 0 度~360 度
    short headingrate;					// -20 度/秒~20度/秒
    short rool;							// -180 度~180度
    short rollrate;						// -20 度/秒~20度/秒
    short pitch;						// -180 度~180度
    short pitchrate;					// -20 度/秒~20度/秒
    short surge;						// -327 米~327米
    short surgespeed;					// -32.767 米/秒~32.768 米/秒
    short sway;							// -327 米~327米
    short swayspeed;					// -32.767 米/秒~32.768 米/秒
    short heave;						// -327 米~327米
    short heavespeed;					// -32.767 米/秒~32.768 米/秒
    short northspeed;					// -32.767 米/秒~32.768 米/秒
    short eastspeed;					// -32.767 米/秒~32.768 米/秒
    short verticalspeed;				// -32.767 米/秒~32.768 米/秒
    short x1acceleration;				// -0.5~0.5g
    short x2acceleration;				// -0.5~0.5g
    short x3acceleration;				// -0.5~0.5g
    int longitude;						// -180~180 度
    int latitude;						// -90~90 度
    int altitude;						// 0.01 米
}SinsData;

// 惯导数据包
typedef struct {
    char* sign;
    unsigned short id;
    unsigned short userid;
    unsigned int counter;
    unsigned int length;
    unsigned short number;
    std::vector<SinsData> vectorsins;
}SinsPackage;

// GNSS子体数据包
typedef struct {
    unsigned int date;
    unsigned int time;						// 0.001 秒
    unsigned short degreeoflongitude; 		//
    unsigned int pointoflongitude; 			// 0.0001′
    unsigned short degreeoflatitude; 		// 0 度~360 度
    unsigned int pointoflatitude; 			// 0.0001′
    short altitude; 						// 1cm
    unsigned short speed; 					// 0.01m/s
    unsigned short heading; 				// 0.1°
    unsigned int numberofsatellites; 		//
    unsigned int status; 					//
    char longitudeidentification; 			// E（东经）/W（西经）
    char latitudeidentification; 			// N（北纬）/S（南纬）
}GnssData;

// GNSS 数据包
typedef struct {
    char* sign;
    unsigned short id;
    unsigned short userid;
    unsigned int counter;
    unsigned int length;
    unsigned int number;
    std::vector<GnssData> vectorgnss;
}GnssPackage;

// 表面声速子体数据包
typedef struct {
    unsigned int date;
    unsigned int time;					// 0.001 秒
    unsigned int speed; 				// 0.01m/s
    short temperature; 		// 0.001℃
    unsigned short pressure; 			// 0.1bars
}SoundSpeedData;

// 表面声速数据包
typedef struct {
    char* sign;
    unsigned short id;
    unsigned short userid;
    unsigned int counter;
    unsigned int length;
    unsigned int number;
    std::vector<SoundSpeedData> vectorsoundspeed;
}SoundSpeedPackage;

// 业务结构
typedef struct {
    double min;
    double max;
    int total_package; // 总包数
    int seabed_package; // 地形包数
    vector<vector<Point*>> points;
}DatSonaFileDetails;

#endif // DEFINES_H
