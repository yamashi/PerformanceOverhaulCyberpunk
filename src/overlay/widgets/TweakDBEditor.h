#pragma once

#include "Widget.h"

struct LuaVM;

struct TweakDBEditor : Widget
{
    TweakDBEditor(LuaVM& aVm);
    ~TweakDBEditor() override = default;

    bool OnEnable() override;
    bool OnDisable() override;
    void Update() override;

protected:
    void RefreshAll();
    void RefreshRecords();
    void RefreshFlats();
    void FilterAll();
    void FilterRecords(bool aFilterTab = true, bool aFilterDropdown = false);
    void FilterFlats();
    bool DrawRecordDropdown(const char* acpLabel, TweakDBID& aDBID, float aWidth = 0);

    static std::string GetTweakDBIDStringRecord(TweakDBID aDBID);
    static bool GetTweakDBIDStringRecord(TweakDBID aDBID, std::string& aString);
    static std::string GetTweakDBIDStringFlat(TweakDBID aDBID);
    static bool GetTweakDBIDStringFlat(TweakDBID aDBID, std::string& aString);
    static std::string GetTweakDBIDStringQuery(TweakDBID aDBID);
    static bool GetTweakDBIDStringQuery(TweakDBID aDBID, std::string& aString);

    bool DrawFlat(TweakDBID aDBID);
    bool DrawFlat(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);
    bool DrawFlatArray(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false,
                       bool aCollapsable = true);
    bool DrawFlatTweakDBID(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);
    static bool DrawFlatQuaternion(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);
    static bool DrawFlatEulerAngles(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);
    static bool DrawFlatVector3(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);
    static bool DrawFlatVector2(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);
    static bool DrawFlatColor(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);
    static bool DrawFlatLocKeyWrapper(TweakDBID aDBID, RED4ext::CStackType& aStackType,
                                      bool aReadOnly = false);
    static bool DrawFlatResourceAsyncRef(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);
    static bool DrawFlatCName(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);
    static bool DrawFlatBool(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);
    static bool DrawFlatString(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);
    static bool DrawFlatFloat(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);
    static bool DrawFlatInt32(TweakDBID aDBID, RED4ext::CStackType& aStackType, bool aReadOnly = false);

    void DrawRecordsTab();
    void DrawQueriesTab();
    void DrawFlatsTab();
    void DrawAdvancedTab();

private:
    // like ImGuiListClipper but supports dynamic size
    struct ImGuiVisibilityChecker
    {
        bool IsVisible(bool aClaimSpaceIfInvisible = true) const;
        void Begin();
        void End();

    private:
        ImVec2 m_itemSize;
        float m_beginCursorY;
    };
    
    struct CachedFlat
    {
        bool m_isFiltered = false;
        bool m_isMissing = false;
        std::string m_name;
        TweakDBID m_dbid;
        ImGuiVisibilityChecker m_visibilityChecker;

        CachedFlat(std::string aName, TweakDBID aDBID) noexcept;
        CachedFlat(CachedFlat&&) noexcept = default;
        CachedFlat& operator=(CachedFlat&&) noexcept = default;
        void Update(int32_t aTDBOffset = -1);
    };

    struct CachedFlatGroup
    {
        bool m_isFiltered = false;
        bool m_isInitialized = false;
        std::string m_name;
        TiltedPhoques::Vector<CachedFlat> m_flats;
        ImGuiVisibilityChecker m_visibilityChecker;

        CachedFlatGroup(std::string aName) noexcept;
        CachedFlatGroup(CachedFlatGroup&&) noexcept = default;
        CachedFlatGroup& operator=(CachedFlatGroup&&) noexcept = default;
        void Initialize();
    };

    struct CachedRecord
    {
        bool m_isFiltered = false;
        bool m_isDropdownFiltered = false;
        bool m_isInitialized = false;
        std::string m_name;
        TweakDBID m_dbid;
        TiltedPhoques::Vector<CachedFlat> m_flats;
        ImGuiVisibilityChecker m_visibilityChecker;

        CachedRecord(std::string aName, TweakDBID aDBID) noexcept;
        CachedRecord(CachedRecord&&) noexcept = default;
        CachedRecord& operator=(CachedRecord&&) noexcept = default;
        void Initialize();
        void InitializeFlats();
        void Update() const;
    };

    struct CachedRecordGroup
    {
        bool m_isFiltered = false;
        bool m_isInitialized = false;
        std::string m_name;
        RED4ext::CName m_typeName;
        TiltedPhoques::Vector<CachedRecord> m_records;
        ImGuiVisibilityChecker m_visibilityChecker;

        CachedRecordGroup(RED4ext::CName aTypeName);
        CachedRecordGroup(CachedRecordGroup&&) noexcept = default;
        CachedRecordGroup& operator=(CachedRecordGroup&&) noexcept = default;
        void Initialize();
    };

    LuaVM& m_vm;
    bool m_initialized = false;
    int32_t m_flatGroupNameDepth = 1;
    TiltedPhoques::Vector<CachedFlatGroup> m_cachedFlatGroups;
    TiltedPhoques::Vector<CachedRecordGroup> m_cachedRecords;
    static bool s_recordsFilterIsRegex;
    static bool s_flatsFilterIsRegex;
    static char s_recordsFilterBuffer[256];
    static char s_flatsFilterBuffer[256];
    static char s_tweakdbidFilterBuffer[256];
};
