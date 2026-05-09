#include "Screen.h"

namespace ProTrack {

// Static member definitions
Investor* Screen::currentInvestor = nullptr;
FundManager* Screen::fundManager = nullptr;
AppScreen Screen::nextScreen = AppScreen::LOGIN;
bool Screen::shouldQuit = false;
std::string Screen::notification = "";
float Screen::notificationTimer = 0;

} // namespace ProTrack
