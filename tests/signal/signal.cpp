#include <signal/signal.hpp>

// C stdlib
#include <cassert>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>

// Containers
#include <string>

static std::string StringPrintf(const char *format, ...) {
    std::string result;
    char str[1000];
    va_list args;
    va_start(args, format);
    if (vsnprintf(str, sizeof str, format, args) >= 0) {
        result = str;
    }
    va_end(args);
    return result;
}

static uint64_t TimestampBenchmark() {
    auto now = static_cast<double>(std::clock());
    return static_cast<uint64_t>(1.0e9 /  CLOCKS_PER_SEC * now);
}

struct TestCounter {
    static uint64_t Get();
    static void Set(uint64_t);
    static void Add2(void *, uint64_t);
};

namespace {                                                       // Anon
void (*test_counter_add2)(void *, uint64_t) = TestCounter::Add2;  // external symbol to prevent easy inlining
static uint64_t test_counter_var = 0;
}  // namespace

class BasicSignalTests {
    static std::string accu;
    struct Foo {
        char FooBool(float f, int i, std::string s) {
            float to_print = f + static_cast<float>(static_cast<size_t>(i) + s.size());
            accu += StringPrintf("Foo: %.2f\n", static_cast<double>(to_print));
            return true;
        }
    };
    static char FloatCallback(float f, int, std::string) {
        accu += StringPrintf("float: %.2f\n", static_cast<double>(f));
        return 0;
    }

public:
    static void Run() {
        accu = "";
        signal::Signal<char(float, int, std::string)> sig1;
        size_t id1 = sig1.Connect(FloatCallback);
        size_t id2 = sig1.Connect([](float, int i, std::string) {
            accu += StringPrintf("int: %d\n", i);
            return 0;
        });
        size_t id3 = sig1.Connect([](float, int, const std::string &s) {
            accu += StringPrintf("string: %s\n", s.c_str());
            return 0;
        });
        sig1.Emit(.3f, 4, "huhu");
        bool success;
        success = sig1.Disconnect(id1);
        assert(success == true);
        success = sig1.Disconnect(id1);
        assert(success == false);
        success = sig1.Disconnect(id2);
        assert(success == true);
        success = sig1.Disconnect(id3);
        assert(success == true);
        success = sig1.Disconnect(id3);
        assert(success == false);
        success = sig1.Disconnect(id2);
        assert(success == false);
        Foo foo;
        sig1.Connect(signal::Slot(foo, &Foo::FooBool));
        sig1.Connect(signal::Slot(&foo, &Foo::FooBool));
        sig1.Emit(.5, 1, "12");

        signal::Signal<void(std::string, int)> sig2;
        sig2.Connect([](std::string msg, int) { accu += StringPrintf("msg: %s", msg.c_str()); });
        sig2.Connect([](std::string, int d) { accu += StringPrintf(" *%d*\n", d); });
        sig2.Emit("in sig2", 17);

        accu += "DONE";

        const char *expected =
            "float: 0.30\n"
            "int: 4\n"
            "string: huhu\n"
            "Foo: 3.50\n"
            "Foo: 3.50\n"
            "msg: in sig2 *17*\n"
            "DONE";
        assert(accu == expected);
    }
};
std::string BasicSignalTests::accu;

class TestCollectorVector {
    static int Handler1() {
        return 1;
    }
    static int Handler42() {
        return 42;
    }
    static int Handler777() {
        return 777;
    }

public:
    static void Run() {
        signal::Signal<int(), signal::CollectorVector<int>> sig_vector;
        sig_vector.Connect(Handler777);
        sig_vector.Connect(Handler42);
        sig_vector.Connect(Handler1);
        sig_vector.Connect(Handler42);
        sig_vector.Connect(Handler777);
        std::vector<int> results = sig_vector.Emit();
        const std::vector<int> reference = {
            777, 42, 1, 42, 777,
        };
        assert(5 == sig_vector.Size());
        assert(results == reference);
    }
};

class TestCollectorWhileTrue {
    bool check1, check2;
    TestCollectorWhileTrue()
      : check1(0)
      , check2(0) {
    }
    bool HandlerTrue() {
        check1 = true;
        return true;
    }
    bool HandlerFalse() {
        check2 = true;
        return false;
    }
    bool HandlerAbort() {
        std::abort();
    }

public:
    static void Run() {
        TestCollectorWhileTrue self;
        signal::Signal<bool(), signal::CollectorWhileTrue<bool>> sig_until0;
        sig_until0.Connect(signal::Slot(self, &TestCollectorWhileTrue::HandlerTrue));
        sig_until0.Connect(signal::Slot(self, &TestCollectorWhileTrue::HandlerFalse));
        sig_until0.Connect(signal::Slot(self, &TestCollectorWhileTrue::HandlerAbort));
        assert(!self.check1 && !self.check2);
        const bool result = sig_until0.Emit();
        assert(!result && self.check1 && self.check2);
    }
};

class TestCollectorWhileFalse {
    bool check1, check2;
    TestCollectorWhileFalse()
      : check1(0)
      , check2(0) {
    }
    bool handler_0() {
        check1 = true;
        return false;
    }
    bool handler_1() {
        check2 = true;
        return true;
    }
    bool HandlerAbort() {
        std::abort();
    }

public:
    static void Run() {
        TestCollectorWhileFalse self;
        signal::Signal<bool(), signal::CollectorWhileFalse<bool>> sig_while0;
        sig_while0.Connect(signal::Slot(self, &TestCollectorWhileFalse::handler_0));
        sig_while0.Connect(signal::Slot(self, &TestCollectorWhileFalse::handler_1));
        sig_while0.Connect(signal::Slot(self, &TestCollectorWhileFalse::HandlerAbort));
        assert(!self.check1 && !self.check2);
        const bool result = sig_while0.Emit();
        assert(result == true && self.check1 && self.check2);
    }
};

static void BenchSignal() {
    signal::Signal<void(void *, uint64_t)> sig_increment;
    sig_increment.Connect(test_counter_add2);
    const uint64_t start_counter = TestCounter::Get();
    const uint64_t benchstart = TimestampBenchmark();
    uint64_t i;
    for (i = 0; i < 999999; i++) {
        sig_increment.Emit(nullptr, 1);
    }
    const uint64_t benchdone = TimestampBenchmark();
    const uint64_t end_counter = TestCounter::Get();
    assert(end_counter - start_counter == i);
    printf("OK\n  Benchmark: signal::Signal: %fns per emission (size=%u): ",
           static_cast<double>((benchdone - benchstart)) * 1.0 / static_cast<double>(i),
           static_cast<unsigned int>(sizeof(sig_increment)));
}

static void BenchCallbackLoop() {
    void (*counter_increment)(void *, uint64_t) = test_counter_add2;
    const uint64_t start_counter = TestCounter::Get();
    const uint64_t benchstart = TimestampBenchmark();
    uint64_t i;
    for (i = 0; i < 999999; i++) {
        counter_increment(nullptr, 1);
    }
    const uint64_t benchdone = TimestampBenchmark();
    const uint64_t end_counter = TestCounter::Get();
    assert(end_counter - start_counter == i);
    printf("OK\n  Benchmark: callback loop: %fns per round: ", size_t(benchdone - benchstart) * 1.0 / size_t(i));
}

uint64_t TestCounter::Get() {
    return test_counter_var;
}

void TestCounter::Set(uint64_t v) {
    test_counter_var = v;
}

void TestCounter::Add2(void *, uint64_t v) {
    test_counter_var += v;
}

int main() {
    printf("Signal/Basic Tests: ");
    BasicSignalTests::Run();
    printf("OK\n");

    printf("Signal/CollectorVector: ");
    TestCollectorVector::Run();
    printf("OK\n");

    printf("Signal/CollectorUntil0: ");
    TestCollectorWhileTrue::Run();
    printf("OK\n");

    printf("Signal/CollectorWhile0: ");
    TestCollectorWhileFalse::Run();
    printf("OK\n");

    printf("Signal/Benchmark: signal::Signal: ");
    BenchSignal();
    printf("OK\n");

    printf("Signal/Benchmark: callback loop: ");
    BenchCallbackLoop();
    printf("OK\n");

    return 0;
}
