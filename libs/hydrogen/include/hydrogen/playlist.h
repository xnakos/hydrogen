/*
 * Hydrogen
 * Copyright(c) 2002-2008 by Alex >Comix< Cominu [comix@users.sourceforge.net]
 *
 * http://www.hydrogen-music.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QDialog>
#include <hydrogen/Object.h>
#include <hydrogen/globals.h>
#include <hydrogen/Preferences.h>
#include <hydrogen/hydrogen.h>
#include "gui/src/HydrogenApp.h"
#include <vector>
//class HydrogenApp;




///
/// handle playlist  
///

class Playlist :  public Object

{
	
	public:
		static Playlist* get_instances();
		
		~Playlist();

//		std::vector<HPlayListNode> m_PlayList;
		void setNextSongByNumber(int SongNumber);	
		void setNextSongPlaylist();
		void setPrevSongPlaylist();
		int selectedSongNumber;
		void setSelectedSongNr( int songNumber);
		int getSelectedSongNr();
		void setActiveSongNumber( int ActivSongNumber);
		int getActiveSongNumber();



	private:

		static Playlist* instance;

	/// Constructor
		Playlist();
		void loadSong( QString songName );
		void execScript( int index);
};


#endif