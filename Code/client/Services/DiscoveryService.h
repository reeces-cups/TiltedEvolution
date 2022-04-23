#pragma once

#include <Events/EventDispatcher.h>
#include <Games/Events.h>

struct World;

struct ConnectedEvent;
struct GameService;

struct PreUpdateEvent;
struct ReferenceSpawnedEvent;

struct TESObjectREFR;
struct TESWorldSpace;

/*!
    Responsible for discovering loaded actors, dispatching cell changes,
    and notifying the rest of the client about newly discovered actors.
*/
struct DiscoveryService final
    : BSTEventSink<TESLoadGameEvent>
{
    DiscoveryService(World& aWorld, entt::dispatcher& aDispatcher) noexcept;
    ~DiscoveryService() noexcept = default;

    TP_NOCOPYMOVE(DiscoveryService);

protected:
    /**
    * Check whether the cell has changed, and if so,
    * notify the client about the new location.
    * @param aForceTrigger Forcefully notify the client about the current location
    */
    void VisitCell(bool aForceTrigger = false) noexcept;
    /**
    * Check whether the grid cell has shifted, and if so,
    * notify the client about the newly loaded cells.
    * @param aWorldSpace The world space that the grid cell is in.
    * @param aNewGridCell True if the player just walked into the new world space.
    */
    void DetectGridCellChange(TESWorldSpace* aWorldSpace, bool aNewGridCell) noexcept;
    /**
    * Dispatch (un)loaded actors.
    */
    void VisitForms() noexcept;

    /**
    * On each update, check whether the actor list or locational data has changed.
    * @param acPreUpdateEvent Update on PreUpdateEvent so that the actors are dispatched before anything else.
    */
    void OnUpdate(const PreUpdateEvent& acPreUpdateEvent) noexcept;

    /**
    * On connect, forcefully dispatch the loaded actors and locational data.
    */
    void OnConnected(const ConnectedEvent& acEvent) noexcept;

    /**
    * When the game finishes loading, forcefully dispatch the loaded actors and locational data.
    * This is a fix for the event that the player dies, and needs to rebroadcast its data.
    * TODO(cosideci): this system is incomplete.
    */
    BSTEventResult OnEvent(const TESLoadGameEvent*, const EventDispatcher<TESLoadGameEvent>*) override;

private:

    /**
    * Reset cached exterior cell data.
    * TODO(cosideci): confirm the usage of this function
    */
    void ResetCachedCellData() noexcept;
    /**
    * Check whether the exterior cell grid has changed, and if so,
    * notify the client about the new location.
    * @param aForceTrigger Forcefully notify the client about the current location
    */
    void VisitExteriorCell(bool aForceTrigger) noexcept;
    /**
    * Check whether the interior cell has changed, and if so,
    * notify the client about the new location.
    * @param aForceTrigger Forcefully notify the client about the current location
    */
    void VisitInteriorCell(bool aForceTrigger) noexcept;

    World& m_world;
    entt::dispatcher& m_dispatcher;

    //! @brief Cached actor forms detected in the previous frame.
    Set<uint32_t> m_forms;
    int32_t m_centerGridX = 0x7FFFFFFF;
    int32_t m_centerGridY = 0x7FFFFFFF;
    int32_t m_currentGridX = 0x7FFFFFFF;
    int32_t m_currentGridY = 0x7FFFFFFF;
    uint32_t m_worldSpaceId = 0;
    uint32_t m_interiorCellId = 0;
    struct TESForm *m_pLocation = nullptr;

    entt::scoped_connection m_preUpdateConnection;
    entt::scoped_connection m_connectedConnection;
};
