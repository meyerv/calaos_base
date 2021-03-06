/******************************************************************************
**  Copyright (c) 2006-2011, Calaos. All Rights Reserved.
**
**  This file is part of Calaos Home.
**
**  Calaos Home is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 3 of the License, or
**  (at your option) any later version.
**
**  Calaos Home is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Calaos; if not, write to the Free Software
**  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
******************************************************************************/
#ifndef AUDIOMODEL_H
#define AUDIOMODEL_H

#include <Utils.h>

#include "CalaosConnection.h"

using namespace Utils;

typedef sigc::slot<void, Params &> PlayerInfo_cb;
typedef sigc::signal<void, Params &> PlayerInfo_signal;
typedef sigc::slot<void, list<Params> &> PlayerInfoList_cb;
typedef sigc::signal<void, list<Params> &> PlayerInfoList_signal;

class IOBase;

class PlayerInfoData
{
        public:
                PlayerInfo_cb callback;
                PlayerInfoList_cb callback_list;

                Params item;
                string cover_fname;
                Ecore_Exe *thumb_exe;
};

class AudioPlayer: public sigc::trackable
{
        private:
                CalaosConnection *connection;

                int volume;
                int changeReg;
                int playlist_size;

                EcoreTimer *timer_change;
                bool time_inprocess;

                double duration;
                double elapsed_time;

                Params db_stats;

                map<int, string> amplifier_inputs;

                string itemTypeToString(int type);
                int itemStringToType(string type);

                void timerChangeTick();

                void notifyChange(string notif);

                void audio_volume_get_cb(bool success, vector<string> result, void *data);
                void audio_status_get_cb(bool success, vector<string> result, void *data);
                void audio_songinfo_get_cb(bool success, vector<string> result, void *data);
                void audio_time_get_cb(bool success, vector<string> result, void *data);
                void audio_playlist_size_get_cb(bool success, vector<string> result, void *data);
                void audio_playlist_size_get_added_cb(bool success, vector<string> result, void *data);
                void playlist_item_get_cb(bool success, vector<string> result, void *data);
                void cover_cb(bool success, vector<string> result, void *data);
                void audio_db_stats_get_cb(bool success, vector<string> result, void *data);

                void db_default_item_get_cb(bool success, vector<string> result, void *data);
                void db_default_item_list_get_cb(bool success, vector<string> result, void *data);

                void db_album_track_count_get_cb(bool success, vector<string> result, void *data);

                void getInputSource_cb(bool success, vector<string> result, void *data);
                void getAmpStates_cb(bool success, vector<string> result, void *data);

        public:
                AudioPlayer(CalaosConnection *c):
                        connection(c),
                        changeReg(0),
                        timer_change(NULL),
                        time_inprocess(false)
                {
                        connection->getListener()->notify_audio_change.connect(
                                                sigc::mem_fun(*this, &AudioPlayer::notifyChange));
                }

                void audio_get_cb(bool success, vector<string> result, void *data);

                Params params;

                int getVolume() { return volume; }
                void setVolume(int volume);
                double getDuration() { return duration; }
                double getTimeElapsed() { return elapsed_time; }
                void setTime(double t);

                string getStatus();

                int getPlaylistSize() { return playlist_size; }

                void play();
                void pause();
                void stop();
                void next();
                void previous();
                void on();
                void off();

                //each view wanting player updates need to register/unregister, so that
                //the model can optimize network queries.
                void registerChange();
                void unregisterChange();

                void playItem(int item);
                void getPlaylistItem(int item, PlayerInfo_cb callback);
                void removePlaylistItem(int item);

                //Add or play an item from the database
                enum { DB_ITEM_NONE = 0, DB_ITEM_TRACK, DB_ITEM_ALBUM, DB_ITEM_ARTIST,
                       DB_ITEM_GENRE, DB_ITEM_YEAR, DB_ITEM_PLAYLIST, DB_ITEM_FOLDER,
                       DB_ITEM_RADIO, DB_ITEM_DIRECTURL };
                void playItem(int type, string id);
                void addItem(int type, string id);

                bool hasCover();
                void getCurrentCover(PlayerInfo_cb callback);

                //Amplifier
                IOBase *getAmplifier();
                map<int, string> getInputSources() { return amplifier_inputs; }
                string getAmplifierStatus(string key);

                //Database queries
                Params &getDBStats();

                void playlistDelete(string id);

                void getDBAlbumItem(int item, PlayerInfo_cb callback);
                void getDBArtistItem(int item, PlayerInfo_cb callback);
                void getDBYearItem(int item, PlayerInfo_cb callback);
                void getDBGenreItem(int item, PlayerInfo_cb callback);
                void getDBPlaylistItem(int item, PlayerInfo_cb callback);

                void getDBAlbumTrackCount(int album_id, PlayerInfo_cb callback);
                void getDBArtistAlbumCount(int artist_id, PlayerInfo_cb callback);
                void getDBYearAlbumCount(int year_id, PlayerInfo_cb callback);
                void getDBGenreArtistCount(int genre_id, PlayerInfo_cb callback);
                void getDBPlaylistTrackCount(int playlist_id, PlayerInfo_cb callback);

                void getDBAlbumTrackItem(int album_id, int item, PlayerInfo_cb callback);
                void getDBAlbumArtistItem(int item, int artist_id, PlayerInfo_cb callback);
                void getDBAlbumYearItem(int item, int year_id, PlayerInfo_cb callback);
                void getDBArtistGenreItem(int item, int genre_id, PlayerInfo_cb callback);
                void getDBPlaylistTrackItem(int playlist_id, int item, PlayerInfo_cb callback);

                void getDBFolder(string folder_id, PlayerInfoList_cb callback);
                void getDBTrackInfos(string track_id, PlayerInfo_cb callback);
                void getDBAllRadio(PlayerInfoList_cb callback);
                void getDBRadio(string radio_id, string subitem_id, PlayerInfoList_cb callback);
                void getDBRadioSearch(string radio_id, string subitem_id, string search, PlayerInfoList_cb callback);
                void getDBSearch(string search, PlayerInfoList_cb callback);

                enum { AUDIO_COVER_SIZE_NONE = 0, AUDIO_COVER_SIZE_SMALL, AUDIO_COVER_SIZE_MEDIUM, AUDIO_COVER_SIZE_BIG };
                void getDBAlbumCoverItem(Params &item, PlayerInfo_cb callback, int size = AUDIO_COVER_SIZE_NONE);

                Params current_song_info;

                sigc::signal<void, AudioPlayer *> load_done;

                //Public notification of player changes
                sigc::signal<void> player_status_changed;
                sigc::signal<void> player_track_changed;
                sigc::signal<void> player_volume_changed;
                sigc::signal<void> player_time_changed;

                //Playlist notification
                sigc::signal<void> player_playlist_changed;
                sigc::signal<void, int> player_playlist_tracks_added;
                sigc::signal<void, int> player_playlist_tracks_deleted;
                sigc::signal<void, int, int> player_playlist_tracks_moved;
};

class AudioModel: public sigc::trackable
{
        private:
                CalaosConnection *connection;

                int load_count;
                void load_audio_done(AudioPlayer *audio);

                void audio_count_cb(bool success, vector<string> result, void *data);

        public:
                AudioModel(CalaosConnection *connection);
                ~AudioModel();

                void load();

                list<AudioPlayer *> players;

                sigc::signal<void> load_done;

                void executableDone(Ecore_Exe_Event_Del *event);
};

#endif // AUDIOMODEL_H
