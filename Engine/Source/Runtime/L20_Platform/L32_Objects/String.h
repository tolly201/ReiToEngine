#ifndef PLATFORM_OBJECTS_STRING_H
#define PLATFORM_OBJECTS_STRING_H
#include "L0_Macro/Include.h"

#include <string>
#include <string_view>
#include <utility>
#include <ostream>
#include <cstddef>
#include <algorithm>

namespace ReiToEngine {

class RTENGINE_API String {
public:
    using value_type             = char;
    using traits_type            = std::char_traits<char>;
    using allocator_type         = std::allocator<char>;
    using size_type              = std::string::size_type;
    using difference_type        = std::string::difference_type;
    using reference              = char&;
    using const_reference        = const char&;
    using pointer                = char*;
    using const_pointer          = const char*;
    using iterator               = std::string::iterator;
    using const_iterator         = std::string::const_iterator;
    using reverse_iterator       = std::string::reverse_iterator;
    using const_reverse_iterator = std::string::const_reverse_iterator;
    struct FmtTag { };                 // 区分的标签类型
    static constexpr FmtTag kFmtTag{};  // 便捷常量
private:
    std::string data_;
    static std::string VFormatImpl(const char* fmt, va_list ap_in) {
        std::string result;
        if(!fmt) return result;

        va_list ap;
        va_copy(ap, ap_in);

        // 初始尝试大小
        char local[256];
        int needed = std::vsnprintf(local, sizeof(local), fmt, ap);
        va_end(ap);

        if(needed < 0) {
            return result; // 失败返回空
        }
        if(static_cast<size_t>(needed) < sizeof(local)) {
            // 直接使用局部缓冲
            result.assign(local, static_cast<size_t>(needed));
            return result;
        }

        // 需要更大缓冲
        size_t size = static_cast<size_t>(needed) + 1; // 包含 '\0'
        std::string buf;
        buf.resize(size);

        va_list ap2;
        va_copy(ap2, ap_in);
        int needed2 = std::vsnprintf(buf.data(), size, fmt, ap2);
        va_end(ap2);

        if(needed2 < 0) {
            return result;
        }
        if(static_cast<size_t>(needed2) + 1 < size) {
            // (极少出现) vsnprintf 返回比预估更小
            buf.resize(static_cast<size_t>(needed2) + 1);
        }
        // 去掉末尾 '\0'
        buf.pop_back();
        return buf;
    }
public:
    // Constructors
    String() = default;
    String(const String&) = default;
    String(String&&) noexcept = default;
    String(const char* s) : data_(s ? s : "") {}
    String(const char* s, size_type count) : data_(s, count) {}
    String(std::string s) : data_(std::move(s)) {}
    String(std::string_view sv) : data_(sv) {}
    String(size_type count, char ch) : data_(count, ch) {}
    String(FmtTag , const char* fmt, ...) {
        if(!fmt) { return; }
        va_list ap;
        va_start(ap, fmt);
        data_ = VFormatImpl(fmt, ap);
        va_end(ap);
    }

    // 静态格式化：String s = String::Format("v=%d", 5);
    static String Format(const char* fmt, ...) {
        String out;
        if(!fmt) return out;
        va_list ap;
        va_start(ap, fmt);
        out.data_ = VFormatImpl(fmt, ap);
        va_end(ap);
        return out;
    }

    // 允许已有 va_list 复用
    static String VFormat(const char* fmt, va_list ap_in) {
        String out;
        if(!fmt) return out;
        va_list ap;
        va_copy(ap, ap_in);
        out.data_ = VFormatImpl(fmt, ap);
        va_end(ap);
        return out;
    }

    template<class It>
    String(It first, It last) : data_(first, last) {}

    // Assignment
    String& operator=(const String&) = default;
    String& operator=(String&&) noexcept = default;
    String& operator=(const char* s) { data_ = (s ? s : ""); return *this; }
    String& operator=(std::string_view sv) { data_ = sv; return *this; }
    String& operator=(char ch) { data_.assign(1, ch); return *this; }

    // Swap
    void swap(String& other) noexcept { data_.swap(other.data_); }
    friend void swap(String& a, String& b) noexcept { a.swap(b); }

    // Capacity
    size_type size() const noexcept { return data_.size(); }
    size_type length() const noexcept { return data_.length(); }
    size_type capacity() const noexcept { return data_.capacity(); }
    size_type max_size() const noexcept { return data_.max_size(); }
    bool empty() const noexcept { return data_.empty(); }
    void reserve(size_type n) { data_.reserve(n); }
    void shrink_to_fit() { data_.shrink_to_fit(); }

    // Element access
    char* data() noexcept { return data_.data(); }
    const char* data() const noexcept { return data_.data(); }
    const char* c_str() const noexcept { return data_.c_str(); }
    reference operator[](size_type i) noexcept { return data_[i]; }
    const_reference operator[](size_type i) const noexcept { return data_[i]; }
    reference at(size_type i) { return data_.at(i); }
    const_reference at(size_type i) const { return data_.at(i); }
    reference front() { return data_.front(); }
    const_reference front() const { return data_.front(); }
    reference back() { return data_.back(); }
    const_reference back() const { return data_.back(); }

    // Iterators
    iterator begin() noexcept { return data_.begin(); }
    const_iterator begin() const noexcept { return data_.begin(); }
    const_iterator cbegin() const noexcept { return data_.cbegin(); }
    iterator end() noexcept { return data_.end(); }
    const_iterator end() const noexcept { return data_.end(); }
    const_iterator cend() const noexcept { return data_.cend(); }
    reverse_iterator rbegin() noexcept { return data_.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return data_.rbegin(); }
    const_reverse_iterator crbegin() const noexcept { return data_.crbegin(); }
    reverse_iterator rend() noexcept { return data_.rend(); }
    const_reverse_iterator rend() const noexcept { return data_.rend(); }
    const_reverse_iterator crend() const noexcept { return data_.crend(); }

    // Modifiers
    void clear() noexcept { data_.clear(); }
    void push_back(char ch) { data_.push_back(ch); }
    void pop_back() { data_.pop_back(); }

    String& append(std::string_view sv) { data_.append(sv); return *this; }
    String& append(const char* s) { data_.append(s ? s : ""); return *this; }
    String& append(const char* s, size_type n) { data_.append(s, n); return *this; }
    String& append(size_type count, char ch) { data_.append(count, ch); return *this; }
    String& operator+=(std::string_view sv) { data_ += sv; return *this; }
    String& operator+=(const String& rhs) { data_ += rhs.data_; return *this; }
    String& operator+=(const char* s) { data_ += (s ? s : ""); return *this; }
    String& operator+=(char ch) { data_ += ch; return *this; }

    String& assign(std::string_view sv) { data_.assign(sv); return *this; }
    String& assign(const char* s) { data_.assign(s ? s : ""); return *this; }
    String& assign(const char* s, size_type n) { data_.assign(s, n); return *this; }
    String& assign(size_type n, char ch) { data_.assign(n, ch); return *this; }

    String& insert(size_type pos, std::string_view sv) { data_.insert(pos, sv); return *this; }
    String& erase(size_type pos = 0, size_type count = std::string::npos) { data_.erase(pos, count); return *this; }
    String& replace(size_type pos, size_type count, std::string_view sv) { data_.replace(pos, count, sv); return *this; }

    void resize(size_type n) { data_.resize(n); }
    void resize(size_type n, char ch) { data_.resize(n, ch); }

    // Search
    size_type find(std::string_view sv, size_type pos = 0) const { return data_.find(sv, pos); }
    size_type find(char ch, size_type pos = 0) const { return data_.find(ch, pos); }
    size_type rfind(std::string_view sv, size_type pos = std::string::npos) const { return data_.rfind(sv, pos); }
    size_type rfind(char ch, size_type pos = std::string::npos) const { return data_.rfind(ch, pos); }
    size_type find_first_of(std::string_view sv, size_type pos = 0) const { return data_.find_first_of(sv, pos); }
    size_type find_last_of(std::string_view sv, size_type pos = std::string::npos) const { return data_.find_last_of(sv, pos); }

    // Substring
    String substr(size_type pos = 0, size_type count = std::string::npos) const {
        return String(data_.substr(pos, count));
    }

    // View / conversion
    std::string_view view() const noexcept { return std::string_view(data_); }
    operator std::string_view() const noexcept { return view(); }
    const std::string& std_str() const noexcept { return data_; }
    std::string&& move_std() noexcept { return std::move(data_); }

    // Compare
    int compare(std::string_view sv) const noexcept { return data_.compare(sv); }

    bool starts_with(std::string_view sv) const noexcept {
#if __cpp_lib_starts_ends_with
        return data_.starts_with(sv);
#else
        return data_.size() >= sv.size() &&
               std::equal(sv.begin(), sv.end(), data_.begin());
#endif
    }
    bool ends_with(std::string_view sv) const noexcept {
#if __cpp_lib_starts_ends_with
        return data_.ends_with(sv);
#else
        return data_.size() >= sv.size() &&
               std::equal(sv.rbegin(), sv.rend(), data_.rbegin());
#endif
    }

    // Case utilities (简单示例，不做 locale)
    String to_lower() const {
        String out;
        out.data_.reserve(data_.size());
        for(char c : data_) out.data_.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
        return out;
    }
    String to_upper() const {
        String out;
        out.data_.reserve(data_.size());
        for(char c : data_) out.data_.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
        return out;
    }

    // Static helpers
    static String FromStd(const std::string& s) { return String(s); }
    static String FromView(std::string_view sv) { return String(sv); }

    // Relational operators
    friend bool operator==(const String& a, const String& b) noexcept { return a.data_ == b.data_; }
    friend bool operator!=(const String& a, const String& b) noexcept { return !(a == b); }
    friend bool operator<(const String& a, const String& b) noexcept { return a.data_ < b.data_; }
    friend bool operator<=(const String& a, const String& b) noexcept { return a.data_ <= b.data_; }
    friend bool operator>(const String& a, const String& b) noexcept { return a.data_ > b.data_; }
    friend bool operator>=(const String& a, const String& b) noexcept { return a.data_ >= b.data_; }

    friend bool operator==(const String& a, std::string_view b) noexcept { return a.view() == b; }
    friend bool operator==(std::string_view a, const String& b) noexcept { return a == b.view(); }

    // Concatenation
    friend String operator+(const String& a, const String& b) { return String(a.data_ + b.data_); }
    friend String operator+(const String& a, std::string_view b) { return String(a.data_ + std::string(b)); }
    friend String operator+(std::string_view a, const String& b) { return String(std::string(a) + b.data_); }
    friend String operator+(const String& a, const char* b) { return String(a.data_ + std::string(b ? b : "")); }
    friend String operator+(const char* a, const String& b) { return String(std::string(a ? a : "") + b.data_); }

    // Stream
    friend std::ostream& operator<<(std::ostream& os, const String& s) {
        return os << s.data_;
    }
};

} // namespace ReiToEngine

// Optional hash support
namespace std {
    template<>
    struct hash<ReiToEngine::String> {
        size_t operator()(const ReiToEngine::String& s) const noexcept {
            return std::hash<std::string_view>()(s.view());
        }
    };
}

#endif
