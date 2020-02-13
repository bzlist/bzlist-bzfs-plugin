/*
 * MIT License
 *
 * Copyright (c) 2019-2020 The Noah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "bzfsAPI.h"
#include "plugin_utils.h"

#include "easywsclient.hpp"
using easywsclient::WebSocket;

class BZList : public bz_Plugin{
public:
  virtual const char* Name();
  virtual void Init(const char* config);
  virtual void Cleanup();
  virtual void Event(bz_EventData* eventData);

private:
  void send(const char* message);
  void connect();

  const char* KEY;
  const char* ADDRESS;
  WebSocket::pointer ws;
  bool connected = false;
};

BZ_PLUGIN(BZList)

const char* BZList::Name(){
  return "BZList v0.0.1";
}

void BZList::Init(const char* config){
  Register(bz_ePlayerScoreChanged);
  Register(bz_ePlayerJoinEvent);
  Register(bz_ePlayerPartEvent);
  Register(bz_eTeamScoreChanged);

  KEY = "API KEY";
  ADDRESS = bz_getPublicAddr().c_str();

  bz_debugMessagef(2, "BZList: server address => %s", ADDRESS);

  connect();
}

void BZList::Cleanup(){
  ws->close();
  Flush();
}

void BZList::Event(bz_EventData* eventData){
  switch (eventData->eventType){
    case bz_ePlayerScoreChanged: {
      bz_PlayerScoreChangeEventData_V1* data = (bz_PlayerScoreChangeEventData_V1*)eventData;

      bz_BasePlayerRecord *player = bz_getPlayerByIndex(data->playerID);

      std::string json = "{";
      json += "\"event\": \"player_score_changed\",";
      json += "\"player\": {";
      json += bz_format("\"callsign\": \"%s\",", player->callsign.c_str());
      json += bz_format("\"wins\": %d,", player->wins);
      json += bz_format("\"losses\": %d,", player->losses);
      json += bz_format("\"tks\": %d", player->teamKills);
      json += "}";
      json += "}";

      send(json.c_str());

      bz_freePlayerRecord(player);
    } break;
    case bz_ePlayerJoinEvent: {
      // This event is called each time a player joins the game
      bz_PlayerJoinPartEventData_V1* data = (bz_PlayerJoinPartEventData_V1*)eventData;

      std::string json = "{";
      json += "\"event\": \"player_join\",";
      json += "\"player\": {";
      json += bz_format("\"callsign\": \"%s\",", data->record->callsign.c_str());
      json += bz_format("\"motto\": \"%s\",", bz_getPlayerMotto(data->record->playerID));
      json += bz_format("\"team\": \"%s\",", bzu_GetTeamName(data->record->team));
      json += bz_format("\"wins\": %d,", data->record->wins);
      json += bz_format("\"losses\": %d,", data->record->losses);
      json += bz_format("\"tks\": %d", data->record->teamKills);
      json += "}";
      json += "}";

      send(json.c_str());
    } break;
    case bz_ePlayerPartEvent: {
      // This event is called each time a player leaves a game
      bz_PlayerJoinPartEventData_V1* data = (bz_PlayerJoinPartEventData_V1*)eventData;

      std::string json = "{";
      json += "\"event\": \"player_part\",";
      json += bz_format("\"callsign\": \"%s\"", data->record->callsign.c_str());
      json += "}";

      send(json.c_str());
    } break;
    case bz_eTeamScoreChanged: {
      // This event is called when a team's score changes
      bz_TeamScoreChangeEventData_V1* data = (bz_TeamScoreChangeEventData_V1*)eventData;

      std::string json = "{";
      json += "\"event\": \"team_score_changed\",";
      json += bz_format("\"team\": \"%s\",", bzu_GetTeamName(data->team));
      json += bz_format("\"wins\": \"%d\",", bz_getTeamWins(data->team));
      json += bz_format("\"losses\": \"%d\"", bz_getTeamLosses(data->team));
      json += "}";

      send(json.c_str());
    } break;
    default:
      break;
  }
}

void BZList::send(const char* message){
  if(ws->getReadyState() <= 2){
    connect();
  }

  ws->send(message);
  ws->poll();
}

void BZList::connect(){
  if(connected){
    ws->poll();
  }

  ws = WebSocket::from_url("ws://api.bzlist.net:8080");
  connected = ws->getReadyState() == WebSocket::OPEN;
  bz_debugMessagef(2, "BZList: connected => %s", connected ? "yes" : "no");

  std::string json = "{";
  json += "\"event\": \"auth\",";
  json += bz_format("\"server\": \"%s\",", ADDRESS);
  json += bz_format("\"key\": \"%s\"", KEY);
  json += "}";

  send(json.c_str());
}
