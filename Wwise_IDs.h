/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Audiokinetic Wwise generated include file. Do not edit.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __WWISE_IDS_H__
#define __WWISE_IDS_H__

#include <AK/SoundEngine/Common/AkTypes.h>

namespace AK
{
    namespace EVENTS
    {
        static const AkUniqueID EXPLODE_ENEMY = 3978230001U;
        static const AkUniqueID EXPLODE_PLAYER = 2840822536U;
        static const AkUniqueID FIRE_LASER_PLAYER = 638744943U;
        static const AkUniqueID GLOBAL_AMBIANCE = 2278749975U;
        static const AkUniqueID LASER_HIT_ENEMY = 3166119473U;
        static const AkUniqueID MOVE_ENEMY = 1439539819U;
        static const AkUniqueID MOVE_PLAYER = 4179450590U;
        static const AkUniqueID MOVE_PROJECTILE = 1836686104U;
        static const AkUniqueID SPAWN_ENEMY = 3869301839U;
        static const AkUniqueID STOP_MOVE_ENEMY = 1955298694U;
        static const AkUniqueID STOP_MOVE_PLAYER = 3509184541U;
    } // namespace EVENTS

    namespace STATES
    {
        namespace PLAYER_STATE
        {
            static const AkUniqueID GROUP = 4071417932U;

            namespace STATE
            {
                static const AkUniqueID PLAYER_IDLE = 3643401235U;
                static const AkUniqueID PLAYER_MOVE = 2248092158U;
            } // namespace STATE
        } // namespace PLAYER_STATE

    } // namespace STATES

    namespace GAME_PARAMETERS
    {
        static const AkUniqueID ENEMY_SCALE = 1656210572U;
        static const AkUniqueID ENEMY_SPAWN_TIMER = 1781608917U;
        static const AkUniqueID POS_X = 2761190168U;
        static const AkUniqueID POS_Y = 2761190169U;
        static const AkUniqueID SS_AIR_FEAR = 1351367891U;
        static const AkUniqueID SS_AIR_FREEFALL = 3002758120U;
        static const AkUniqueID SS_AIR_FURY = 1029930033U;
        static const AkUniqueID SS_AIR_MONTH = 2648548617U;
        static const AkUniqueID SS_AIR_PRESENCE = 3847924954U;
        static const AkUniqueID SS_AIR_RPM = 822163944U;
        static const AkUniqueID SS_AIR_SIZE = 3074696722U;
        static const AkUniqueID SS_AIR_STORM = 3715662592U;
        static const AkUniqueID SS_AIR_TIMEOFDAY = 3203397129U;
        static const AkUniqueID SS_AIR_TURBULENCE = 4160247818U;
    } // namespace GAME_PARAMETERS

    namespace BANKS
    {
        static const AkUniqueID INIT = 1355168291U;
        static const AkUniqueID MAIN = 3161908922U;
    } // namespace BANKS

    namespace BUSSES
    {
        static const AkUniqueID ENEMY_MOVEMENT_BUS = 124992084U;
        static const AkUniqueID MASTER_AUDIO_BUS = 3803692087U;
        static const AkUniqueID MASTER_SECONDARY_BUS = 805203703U;
    } // namespace BUSSES

}// namespace AK

#endif // __WWISE_IDS_H__
