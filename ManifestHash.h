#ifndef MANIFEST_HASH_H
#define MANIFEST_HASH_H

#include <string>
#include <unordered_map>

using namespace std;

// Hash table for manifest verification
class ManifestHash {
private:
    unordered_map<string, size_t> registry;

public:
    ManifestHash();
    ~ManifestHash();

    // Core hashing functions
    size_t calculateHash(const string& text) const;
    void registerManifest(const string& manifestId, const string& docContent);
    bool verifyManifest(const string& manifestId, const string& docContent) const;
    void displayManifests() const;
    bool saveToCSV(const string& filename) const;
    bool loadFromCSV(const string& filename);

    void registerDirectHash(const string& manifestId, size_t hashValue);

    void clear() { registry.clear(); }
    size_t size() const { return registry.size(); }
    const unordered_map<string, size_t>& getRegistry() const { return registry; }
};

#endif // MANIFEST_HASH_H
