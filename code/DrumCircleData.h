//--------------------------------------------------------------------
//	
//	DrumCircleData.h
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#ifndef _DrumCircleData_h
#define _DrumCircleData_h

#include <SupportDefs.h>
#include <Archivable.h>

class BMessage;

//====================================================================
//	CLASS: DrumCircleData

class DrumCircleData: public BArchivable
{

	//----------------------------------------------------------------
	//	Virtual member function overrides

public:
	DrumCircleData(const BMessage * dcdmessage);
	DrumCircleData(uint16 measure_length = 12, uint16 drum_count = 16);
	~DrumCircleData();
	virtual status_t Archive(BMessage *archive, bool deep = true) const;
	static BArchivable * Instantiate(BMessage *archive);



	//----------------------------------------------------------------
	//	Member variables

//using (const &) 's to ensure data will not break

private:	
	uint16		p_drumcount;
	uint16		p_measurelen;
	uint16 *	p_drums;
	uint16 *	p_velocity;
	float *		p_duration;
	bool **		p_rythm;					
	uint16 *	p_changelust;
	uint16 *	p_hitlust;
	uint16 *	p_channel;
	bool *		p_on;
	char *		p_name;
		
		
	//----------------------------------------------------------------
	//	Member functions
public:
	void Rename(const char * newname);
	void Setto(const DrumCircleData &);
	void Move(uint16 from, uint16 to);
	//access functions
	inline uint16 Drumcount()							{ return p_drumcount; };
	inline uint16 Measurelen()							{ return p_measurelen; };
	inline uint16 & Drums(uint index)					{ return p_drums[index]; };
	inline uint16 & Velocity(uint index)				{ return p_velocity[index]; };
	inline float & Duration(uint index)					{ return p_duration[index]; };
	inline bool & Rythm(uint indexa, uint indexb)		{ return p_rythm[indexa][indexb]; };
	inline uint16 & Changelust(uint index)				{ return p_changelust[index]; };
	inline uint16 & Hitlust(uint index)					{ return p_hitlust[index]; };
	inline uint16 & Channel(uint index)					{ return p_channel[index]; };
	inline bool & On(uint index)						{ return p_on[index]; };
	inline const char * Name() const 					{ return p_name; };

		
};




#endif