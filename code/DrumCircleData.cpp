//--------------------------------------------------------------------
//	
//	DrumCircleData.cpp
//
//	Written by: Steffen Yount
//	
//	Copyright 1999 Steffen Yount All Rights Reserved.
//	
//--------------------------------------------------------------------

#include "DrumCircleData.h"
#if __POWERPC__
#include "my_strstream.h"
#endif
#if __INTEL__
#include <strstream.h>
#endif
#include <Message.h>
#include <Errors.h>

//====================================================================
//	Helper functions
/*
uint32 longuget(ifstream & in)
{
	unsigned char temp;
	long unsigned int out = 0;
	for (int i = 0; i < 4; i++)
	{
		in.get(temp);
		out = (out << 8) + temp;
	}
	return out;
}

void longuput(ofstream & out, uint32 val)
{
	for (int i = 3; i >= 0; i--)
	{
		out.put((unsigned char)((val >> (8*i)) & 0xff));
	}
}

*/

//====================================================================
//	DrumCircleData Implementation


//--------------------------------------------------------------------
//	DrumCircleData constructors, destructors, operators

DrumCircleData::DrumCircleData(const BMessage * dcdmessage)
	{
	status_t err;
	p_drumcount = dcdmessage->FindInt16("drumcount");
	p_measurelen = dcdmessage->FindInt16("measurelen");
	ssize_t sz;
	uint16 * tu16;
	
	err = dcdmessage->FindData("drums", B_UINT16_TYPE, (const void **)&tu16, &sz); 
	if (sz != Drumcount()*2)
		err |= B_BAD_VALUE;
	if (!err) 
	{
		p_drums = new uint16[Drumcount()];
		for (uint16 i = 0; i < Drumcount(); i++)
			Drums(i) = tu16[i];
	}

	err |= dcdmessage->FindData("velocity", B_UINT16_TYPE, (const void **)&tu16, &sz); 
	if (sz != Drumcount()*2)
		err |= B_BAD_VALUE;
	if (!err) 
	{
		p_velocity = new uint16[Drumcount()];
		for (uint16 i = 0; i < Drumcount(); i++)
			Velocity(i) = tu16[i];
	}
	
	float * tf;
	err |= dcdmessage->FindData("duration", B_FLOAT_TYPE, (const void **)&tf, &sz); 
	if (sz != Drumcount()*4)
		err |= B_BAD_VALUE;
	if (!err) 
	{
		p_duration = new float[Drumcount()];
		for (uint16 i = 0; i < Drumcount(); i++)
			Duration(i) = tf[i];
	}
	
	bool * tb;
	err |= dcdmessage->FindData("rythm", B_BOOL_TYPE, (const void **)&tb, &sz); 
	if (sz != Drumcount()*Measurelen())
		err |= B_BAD_VALUE;
	if (!err) 
	{
		p_rythm = new bool * [Drumcount()];
		for (uint16 i = 0; i < Drumcount(); i++)
		{
			p_rythm[i] = new bool[Measurelen()];
			for (uint16 k = 0; k < Measurelen(); k++)
				Rythm(i, k) = tb[i*Measurelen()+k];
		}
	}
	
	err = dcdmessage->FindData("changelust", B_UINT16_TYPE, (const void **)&tu16, &sz); 
	if (sz != Drumcount()*2)
		err |= B_BAD_VALUE;
	if (!err) 
	{
		p_changelust = new uint16[Drumcount()];
		for (uint16 i = 0; i < Drumcount(); i++)
			Changelust(i) = tu16[i];
	}
	
	err = dcdmessage->FindData("hitlust", B_UINT16_TYPE, (const void **)&tu16, &sz); 
	if (sz != Drumcount()*2)
		err |= B_BAD_VALUE;
	if (!err) 
	{
		p_hitlust = new uint16[Drumcount()];
		for (uint16 i = 0; i < Drumcount(); i++)
			Hitlust(i) = tu16[i];
	}
	
	err = dcdmessage->FindData("channel", B_UINT16_TYPE, (const void **)&tu16, &sz); 
	if (sz != Drumcount()*2)
		err |= B_BAD_VALUE;
	if (!err) 
	{
		p_channel = new uint16[Drumcount()];
		for (uint16 i = 0; i < Drumcount(); i++)
			Channel(i) = tu16[i];
	}

	err |= dcdmessage->FindData("on", B_BOOL_TYPE, (const void **)&tb, &sz); 
	if (sz != Drumcount())
		err |= B_BAD_VALUE;
	if (!err) 
	{
		p_on = new bool[Drumcount()];
		for (uint16 i = 0; i < Drumcount(); i++)
			On(i) = tb[i];
	}
	
	ostrstream buf;
	buf << dcdmessage->FindString("dcd_name") << '\0';
	p_name = buf.str();

	if (err)
	{
		this->~DrumCircleData();
		p_drumcount = 0;
		p_measurelen = 0;
//		cout << "problems constructing DrumCircleData!!!";
	}	
	
	
	/*
	//old copy constructor
	p_drumcount = d.Drumcount();
	p_measurelen = d.Measurelen();
	p_drums = new uint16[d.Drumcount()];
	for (uint16 i = 0; i < d.Drumcount(); i++)
	{
		Drums(i) = d.Drums(i);
	}
	p_velocity = new uint16[d.Drumcount()];
	p_duration = new float[d.Drumcount()];
	p_rythm = new bool * [d.Drumcount()];
	p_changelust = new uint16[d.Drumcount()];
	p_hitlust = new uint16[d.Drumcount()];
	p_channel = new uint16[d.Drumcount()];
	p_on = new bool[d.Drumcount()];

	ostrstream buf;
	buf << d.name << '\0';
	p_name = buf.str();

	for (uint16 i = 0; i < d.Drumcount(); i++)
	{
		p_rythm[i] = new bool[d.Measurelen()];
		for (uint16 k = 0; k < d.Measurelen(); k++)
			Rythm(i, k) = d.Rythm(i, k);
		Velocity(i) = d.Velocity(i);
		Duration(i) = d.Duration(i);
		Changelust(i) = d.Changelust(i);
		Hitlust(i) = d.Hitlust(i);
		Channel(i) = d.Channel(i);
		On(i) = d.On(i);
	}*/
}

DrumCircleData::DrumCircleData( uint16 measure_length, uint16 drum_count)
{
	p_drumcount = drum_count;
	p_measurelen = measure_length;
	const uint16 def_drums[16] = { 0x3c, 0x3d, 0x3e, 0x3f, 0x40,
							0x23, 0x56, 0x38, 0x1f,
							0x2c, 0x36, 0x45, 0x4c, 0x4d, 0x52,
							0x46 };
	p_drums = new uint16[Drumcount()];
	for (uint16 i = 0; i < 16 && i < Drumcount(); i++)
	{
		Drums(i) = def_drums[i];
	}
	for (uint16 i = 16; i < Drumcount(); i++)
	{
		Drums(i) = 0x01;
	}
/*
	p_drums = new uint16[Drumcount()];
	for (uint16 i = 0; i < Drumcount(); i++)
	{
		Drums(i) = i+27;
	}
*/
	p_velocity = new uint16[Drumcount()];
	p_duration = new float[Drumcount()];
	p_rythm = new bool * [Drumcount()];
	p_changelust = new uint16[Drumcount()];
	p_hitlust = new uint16[Drumcount()];
	p_channel = new uint16[Drumcount()];
	p_on = new bool[Drumcount()];

	ostrstream buf;
	buf << "DrumCircle" << '\0';
	p_name = buf.str();

	for (uint16 i = 0; i < Drumcount(); i++)
	{
		p_rythm[i] = new bool[Measurelen()];
		for (uint16 k = 0; k < Measurelen(); k++)
			Rythm(i, k) = false;
		Velocity(i) = 0x63;
		Duration(i) = 0.25;
		Changelust(i) = 10;
		Hitlust(i) = 25;
		Channel(i) = 10;
		On(i) = true;
	}
}



status_t DrumCircleData::Archive(BMessage *archive, bool deep) const
{
	status_t err;
	err = BArchivable::Archive(archive, deep);
	err |= archive->AddString("class", "DrumCircleData"); 
	err |= archive->AddInt16("drumcount", p_drumcount);
	err |= archive->AddInt16("measurelen", p_measurelen);
	err |= archive->AddData("drums", B_UINT16_TYPE, 
         p_drums, p_drumcount*2, true, p_drumcount);
	err |= archive->AddData("velocity", B_UINT16_TYPE, 
         p_velocity, p_drumcount*2, true, p_drumcount);
    err |= archive->AddData("duration", B_FLOAT_TYPE, 
         p_duration, p_drumcount*4, true, p_drumcount);
    bool * temp = new bool[p_drumcount*p_measurelen];
    if (temp == NULL)
    	err |= B_NO_MEMORY;
    else
    {
	    for (uint16 i = 0; i < p_drumcount; i++)
    		for (uint16 j = 0; j < p_measurelen; j++)
    		{
    			temp[i*p_measurelen+j] = p_rythm[i][j];
	    	}
    	err |= archive->AddData("rythm", B_BOOL_TYPE, 
        	 temp, p_drumcount*p_measurelen, true, p_drumcount*p_measurelen);
	    delete temp;
	}
	err |= archive->AddData("changelust", B_UINT16_TYPE, 
         p_changelust, p_drumcount*2, true, p_drumcount);
	err |= archive->AddData("hitlust", B_UINT16_TYPE, 
         p_hitlust, p_drumcount*2, true, p_drumcount);
	err |= archive->AddData("channel", B_UINT16_TYPE, 
         p_channel, p_drumcount*2, true, p_drumcount);
    err |= archive->AddData("on", B_BOOL_TYPE, 
         p_on, p_drumcount, true, p_drumcount);
    err |= archive->AddString("dcd_name", p_name);
	return err;
} 



BArchivable * DrumCircleData::Instantiate(BMessage *archive)
{
	if ( validate_instantiation(archive, "DrumCircleData")) 
		return new DrumCircleData(archive); 
	return NULL; 

}

void DrumCircleData::Rename(const char * newname)
{
	ostrstream buf;
	buf << newname << '\0';
	delete [] p_name;
	p_name = buf.str();
}

void DrumCircleData::Setto(const DrumCircleData & d)
{
	for (uint16 i = 0; i < p_drumcount && i < d.p_drumcount; i++)
	{
		p_drums[i] = d.p_drums[i];
	}
	for (uint16 i = 0; i < p_drumcount && i < d.p_drumcount; i++)
	{
		for (uint16 k = 0; k < p_measurelen && k < d.p_measurelen; k++)
			p_rythm[i][k] = d.p_rythm[i][k];
		p_velocity[i] = d.p_velocity[i];
		p_duration[i] = d.p_duration[i];
		p_changelust[i] = d.p_changelust[i];
		p_hitlust[i] = d.p_hitlust[i];
		p_channel[i] = d.p_channel[i];
		p_on[i] = d.p_on[i];
	}
	ostrstream buf;
	buf << d.Name() << '\0';
	delete [] p_name;
	p_name = buf.str();

}

void DrumCircleData::Move(uint16 from, uint16 to)
{
	if (from < Drumcount() && to < Drumcount())
	{
		uint16 drum = Drums(from);
		uint16 vel = Velocity(from);
		float dur = Duration(from);
		bool * ryth = p_rythm[from];
		uint16 chng = Changelust(from);
		uint16 hit = Hitlust(from);
		uint16 chan = Channel(from);
		bool t_on = On(from);
		
		if (from < to)
		{
			for (uint16 i = from; i < to; i++)
			{
				Drums(i) = Drums(i+1);
				Velocity(i) = Velocity(i+1);
				Duration(i) = Duration(i+1);
				p_rythm[i] = p_rythm[i+1];
				Changelust(i) = Changelust(i+1);
				Hitlust(i) = Hitlust(i+1);
				Channel(i) = Channel(i+1);
				On(i) = On(i+1);
			}	
		}
		else if (from > to)
		{
			for(uint16 i = from; i > to; i--)
			{
				Drums(i) = Drums(i-1);
				Velocity(i) = Velocity(i-1);
				Duration(i) = Duration(i-1);
				p_rythm[i] = p_rythm[i-1];
				Changelust(i) = Changelust(i-1);
				Hitlust(i) = Hitlust(i-1);
				Channel(i) = Channel(i-1);
				On(i) = On(i-1);
			}
		}
		if (from != to)
		{
			Drums(to) = drum;
			Velocity(to) = vel;
			Duration(to) = dur;
			p_rythm[to] = ryth;
			Changelust(to) = chng;
			Hitlust(to) = hit;
			Channel(to) = chan;
			On(to) = t_on;
		}
	}
}
	
DrumCircleData::~DrumCircleData()
{
	delete [] p_drums;
	delete [] p_velocity;
	delete [] p_duration;
	for (uint16 i = 0; i < Drumcount(); i++)
	{
		delete [] p_rythm[i];
	}
	delete [] p_rythm;					
	delete [] p_changelust;
	delete [] p_hitlust;
	delete [] p_channel;
	delete [] p_on;
	delete [] p_name;
}


