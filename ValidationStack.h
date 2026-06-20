#ifndef VALIDATION_STACK_H
#define VALIDATION_STACK_H

#include <string>
#include <vector>

using namespace std;

// Struct to store a validation event log entry
struct ValidationStep {
    string containerId;
    string stepDescription;
    string status; 
    string timestamp;

    ValidationStep(string cid, string desc, string stat, string time)
        : containerId(cid), stepDescription(desc), status(stat), timestamp(time) {}
};

// Stack implementation to track validation history
class ValidationStack {
private:
    vector<ValidationStep> stack;

public:
    ValidationStack();
    ~ValidationStack();

    // Stack operations
    void addValidationStep(const string& containerId, const string& stepDescription, const string& status, const string& timestamp);
    bool undoLastValidation();
    bool viewLatestStep(ValidationStep& outStep) const;
    void displayAllSteps() const;
    bool saveToCSV(const string& filename) const;
    bool loadFromCSV(const string& filename);

    bool isEmpty() const { return stack.empty(); }
    size_t size() const { return stack.size(); }
    void clear() { stack.clear(); }
};

#endif // VALIDATION_STACK_H
