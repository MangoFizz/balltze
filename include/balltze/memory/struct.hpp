#ifndef BALLTZE__MEMORY___STRUCT_HPP
#define BALLTZE__MEMORY___STRUCT_HPP

// Padding
#define GAP(bytes, line) char gap_##line[bytes]
#define GET_GAP(bytes, line) GAP(bytes, line)
#define PADDING(bytes) GET_GAP(bytes, __LINE__)

#endif