#ifdef _WIN32

namespace {
extern "C" int __stdcall SetConsoleCP(unsigned int wCodePageID);
extern "C" int __stdcall SetConsoleOutputCP(unsigned int wCodePageID);
constexpr unsigned int CP_UTF8 = 65001;

int utf8_compat_init = [] {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	return 0;
}();
} // namespace

#endif
