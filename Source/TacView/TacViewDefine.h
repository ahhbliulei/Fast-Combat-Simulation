﻿// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @file           TacViewDefine.h
*   @brief          TacView 通讯中用到的结构。
*   @details        TacView 通讯中用到的结构。
*   @author         GaoYang
*   @date           20181126
*   @version        1.0.0.1
*   @par Copyright
*                   GaoYang
*   @par History
*                   1.0.0.1: GaoYang, 20181126, 首次创建
*                   1.0.0.2: lidaiwei, 20200808, 修正bug
*/

#ifndef Frame_IO_TacView_Define_H
#define Frame_IO_TacView_Define_H

#pragma pack(push)
#pragma pack(1)

//#include "..\..\FrameDefine.h"

#define max_str 1024

// --------------------------------------------------------------------------------------------------------------------------------
/**
*   @brief          TacView 命名空间。
*   @details        TacView 命名空间。
*/
namespace TacView
{
	// --------------------------------------------------------------------------------------------------------------------------------
	/**
	*   @name           TacView 数据长度宏定义。
	*   @{
	*/
	#define max_object 16
	#define max_event 4
	#define net_buffer_size 1024*16
	/** @}  */

	// --------------------------------------------------------------------------------------------------------------------------------
	/**
	*   @brief          战场基本信息。
	*   @details        战场基本信息，详见 ACMI 格式说明。
	*/
	struct Header_T
	{
		char							data_source[max_str];			//!< Source simulator, control station or file format.
		char							data_recorder[max_str];			//!< Software or hardware used to record the data.
		char  							reference_time[max_str];		//!< Base time (UTC) for the current mission. This time is combined with each frame offset (in seconds) to get the final absolute UTC time for each data sample.
		char  							recording_time[max_str];		//!< Recording (file) creation (UTC) time.
		char  							author[max_str];				//!< Author or operator who has created this recording.
		char  							title[max_str];					//!< Mission/flight title or designation.
		char  							category[max_str];				//!< Category of the flight/mission.
		char  							briefing[max_str];				//!< Free text containing the briefing of the flight/mission.
		char  							debriefing[max_str];			//!< Free text containing the debriefing.
		char  							comments[max_str];				//!< Free comments about the flight. Do not forget to escape any end-of-line character you want to inject into the comments.
		double							reference_longitude;			//!< 参考点经度，单位：deg
		double							reference_latitude;				//!< 参考点纬度，单位：deg
	};

	// --------------------------------------------------------------------------------------------------------------------------------
	/**
	*   @brief          对象。
	*   @details        对象，详见 ACMI 格式说明。
	*/
	struct Object_T
	{
		int  							id;								//!< 编号，唯一标识

		int  							live;							//!< 存在，0 不存在，1 存在

		int								coordinate_valid;				//!< 坐标，有效性，0 无效，1 有效
		int								coordinate_type;				//!< 坐标，类型，0 LLA，1 LLAUV，2 LLARPY，3 LLARPYUVH
		double							coordinate_longitude;			//!< 坐标，经度，单位：度
		double							coordinate_latitude;			//!< 坐标，纬度，单位：度
		double							coordinate_altitude;			//!< 坐标，高度，单位：米
		double							coordinate_roll;				//!< 坐标，滚转角，单位：度
		double							coordinate_pitch;				//!< 坐标，俯仰角，单位：度
		double							coordinate_yaw;					//!< 坐标，偏航角，单位：度，原始点：正北，正方向：顺时针
		double							coordinate_u;					//!< 坐标，x，单位：米
		double							coordinate_v;					//!< 坐标，y，单位：米
		double							coordinate_heading;				//!< 坐标，方向，单位：度

		int								base_valid;						//!< 基本信息，有效性，0 无效，1 有效
		char  							base_name[max_str];				//!< 基本信息，必填，对象名字，例：F-16
		char  							base_type[max_str];				//!< 基本信息，必填，类型，例：Air+FixedWing
		char  							base_pilot[max_str];			//!< 基本信息，必填，飞行员
		char  							base_country[max_str];			//!< 基本信息，必填，国家，参考标准：ISO 3166-1 alpha-2，例：CN US RU
		char  							base_coalition[max_str];		//!< 基本信息，必填，联盟，例：同盟 Allies 轴心 Axis
		char  							base_color[max_str];			//!< 基本信息，必填，颜色，例：Red Orange Green Blue Violet
		char  							base_shape[max_str];			//!< 基本信息，不填，模型文件名
		char  							base_label[max_str];			//!< 基本信息，必填，标签
		double							base_length;					//!< 基本信息，必填，长，单位：米
		double							base_width;						//!< 基本信息，必填，宽，单位：米
		double							base_height;					//!< 基本信息，必填，高，单位：米

		int								state_valid;					//!< 状态，有效性，0 无效，1 有效
		int  							state_focus_target;				//!< 状态，焦点 id
		double							state_radius;					//!< 状态，半径，单位：米
		double							state_engagement_range;			//!< 状态，作战半径，单位：米

		int								radar_valid;					//!< 雷达，有效性，0 无效，1 有效
		int								radar_mode;						//!< 雷达，模式，0 = off, 1 = on
		double							radar_azimuth;					//!< 雷达，航向角，单位：度
		double							radar_elevation;				//!< 雷达，俯仰角，单位：度
		double							radar_range;					//!< 雷达，范围，单位：米
		double							radar_horizontal_beamwidth;		//!< 雷达，波束宽度，单位：度
		double							radar_vertical_beamwidth;		//!< 雷达，波束高度，单位：度

		int								locked_target_valid;			//!< 锁定目标，有效性，0 无效，1 有效
		int  							locked_target_id;				//!< 锁定目标，id
		int								locked_target_mode;				//!< 锁定目标，模式，0 = off, 1 = on
		double							locked_target_azimuth;			//!< 锁定目标，航向角，单位：度
		double							locked_target_elevation;		//!< 锁定目标，俯仰角，单位：度
		double							locked_target_range;			//!< 锁定目标，范围，单位：米
	};

	// --------------------------------------------------------------------------------------------------------------------------------
	/**
	*   @brief          事件。
	*   @details        事件，详见 ACMI 格式说明。
	*/
	struct Event_T
	{
		char  							type[max_str];					//!< 类型
		int  							id_list[max_object];			//!< 对象列表
		char  							text[max_str];					//!< 描述
	};

	// --------------------------------------------------------------------------------------------------------------------------------
	/**
	*   @brief          仿真推进内容。
	*   @details        仿真推进内容，详见 ACMI 格式说明。
	*/
	struct State_T
	{
		double							time;							//!< 时标
		int								object_count;					//!< 对象 数量
		Object_T						object[max_object];				//!< 对象 列表
		int								event_count;					//!< 事件 数量
		Event_T  						event[max_event];				//!< 事件 列表
	};
}


#pragma pack(pop)

#endif // Frame_IO_TacView_Define_H
