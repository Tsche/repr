#pragma once
#include <type_traits>

#include <librepr/util/reftuple.h>
#include "arity.h"

namespace librepr::detail {

// NOLINTBEGIN
template <typename T, typename V>
  requires(std::is_aggregate_v<std::remove_cvref_t<T>> && !std::is_array_v<std::remove_cvref_t<T>>)
constexpr auto visit_aggregate(V&& visitor, T&& object) {
  constexpr auto member_count = arity<std::remove_cvref_t<T>>;
  if constexpr (member_count == 0) {
    return visitor();
  } else if constexpr (member_count == 1) {
    auto& [member_0] = object;
    return visitor(member_0);
  } else if constexpr (member_count == 2) {
    auto& [member_0, member_1] = object;
    return visitor(member_0, member_1);
  } else if constexpr (member_count == 3) {
    auto& [member_0, member_1, member_2] = object;
    return visitor(member_0, member_1, member_2);
  } else if constexpr (member_count == 4) {
    auto& [member_0, member_1, member_2, member_3] = object;
    return visitor(member_0, member_1, member_2, member_3);
  } else if constexpr (member_count == 5) {
    auto& [member_0, member_1, member_2, member_3, member_4] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4);
  } else if constexpr (member_count == 6) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5);
  } else if constexpr (member_count == 7) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6);
  } else if constexpr (member_count == 8) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7);
  } else if constexpr (member_count == 9) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8);
  } else if constexpr (member_count == 10) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9);
  } else if constexpr (member_count == 11) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10);
  } else if constexpr (member_count == 12) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11);
  } else if constexpr (member_count == 13) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12);
  } else if constexpr (member_count == 14) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13);
  } else if constexpr (member_count == 15) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14);
  } else if constexpr (member_count == 16) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15);
  } else if constexpr (member_count == 17) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16);
  } else if constexpr (member_count == 18) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17);
  } else if constexpr (member_count == 19) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18);
  } else if constexpr (member_count == 20) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
           member_19] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19);
  } else if constexpr (member_count == 21) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20);
  } else if constexpr (member_count == 22) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21);
  } else if constexpr (member_count == 23) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22);
  } else if constexpr (member_count == 24) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23);
  } else if constexpr (member_count == 25) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24);
  } else if constexpr (member_count == 26) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25);
  } else if constexpr (member_count == 27) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26);
  } else if constexpr (member_count == 28) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27);
  } else if constexpr (member_count == 29) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28);
  } else if constexpr (member_count == 30) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28,
           member_29] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29);
  } else if constexpr (member_count == 31) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30);
  } else if constexpr (member_count == 32) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31);
  } else if constexpr (member_count == 33) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32);
  } else if constexpr (member_count == 34) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33);
  } else if constexpr (member_count == 35) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34);
  } else if constexpr (member_count == 36) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35);
  } else if constexpr (member_count == 37) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36);
  } else if constexpr (member_count == 38) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37);
  } else if constexpr (member_count == 39) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38);
  } else if constexpr (member_count == 40) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38,
           member_39] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39);
  } else if constexpr (member_count == 41) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40);
  } else if constexpr (member_count == 42) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41);
  } else if constexpr (member_count == 43) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42);
  } else if constexpr (member_count == 44) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43);
  } else if constexpr (member_count == 45) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44);
  } else if constexpr (member_count == 46) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45);
  } else if constexpr (member_count == 47) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46);
  } else if constexpr (member_count == 48) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47);
  } else if constexpr (member_count == 49) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48);
  } else if constexpr (member_count == 50) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48,
           member_49] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49);
  } else if constexpr (member_count == 51) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50);
  } else if constexpr (member_count == 52) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51);
  } else if constexpr (member_count == 53) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51, member_52] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51, member_52);
  } else if constexpr (member_count == 54) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51, member_52, member_53] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51, member_52, member_53);
  } else if constexpr (member_count == 55) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51, member_52, member_53, member_54] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51, member_52, member_53, member_54);
  } else if constexpr (member_count == 56) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51, member_52, member_53, member_54, member_55] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51, member_52, member_53, member_54,
                   member_55);
  } else if constexpr (member_count == 57) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51, member_52, member_53, member_54, member_55, member_56] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51, member_52, member_53, member_54,
                   member_55, member_56);
  } else if constexpr (member_count == 58) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51, member_52, member_53, member_54, member_55, member_56, member_57] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51, member_52, member_53, member_54,
                   member_55, member_56, member_57);
  } else if constexpr (member_count == 59) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51, member_52, member_53, member_54, member_55, member_56, member_57, member_58] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51, member_52, member_53, member_54,
                   member_55, member_56, member_57, member_58);
  } else if constexpr (member_count == 60) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51, member_52, member_53, member_54, member_55, member_56, member_57, member_58,
           member_59] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51, member_52, member_53, member_54,
                   member_55, member_56, member_57, member_58, member_59);
  } else if constexpr (member_count == 61) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51, member_52, member_53, member_54, member_55, member_56, member_57, member_58, member_59,
           member_60] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51, member_52, member_53, member_54,
                   member_55, member_56, member_57, member_58, member_59, member_60);
  } else if constexpr (member_count == 62) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51, member_52, member_53, member_54, member_55, member_56, member_57, member_58, member_59,
           member_60, member_61] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51, member_52, member_53, member_54,
                   member_55, member_56, member_57, member_58, member_59, member_60, member_61);
  } else if constexpr (member_count == 63) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51, member_52, member_53, member_54, member_55, member_56, member_57, member_58, member_59,
           member_60, member_61, member_62] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51, member_52, member_53, member_54,
                   member_55, member_56, member_57, member_58, member_59, member_60, member_61, member_62);
  } else if constexpr (member_count == 64) {
    auto& [member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
           member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18, member_19,
           member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27, member_28, member_29,
           member_30, member_31, member_32, member_33, member_34, member_35, member_36, member_37, member_38, member_39,
           member_40, member_41, member_42, member_43, member_44, member_45, member_46, member_47, member_48, member_49,
           member_50, member_51, member_52, member_53, member_54, member_55, member_56, member_57, member_58, member_59,
           member_60, member_61, member_62, member_63] = object;
    return visitor(member_0, member_1, member_2, member_3, member_4, member_5, member_6, member_7, member_8, member_9,
                   member_10, member_11, member_12, member_13, member_14, member_15, member_16, member_17, member_18,
                   member_19, member_20, member_21, member_22, member_23, member_24, member_25, member_26, member_27,
                   member_28, member_29, member_30, member_31, member_32, member_33, member_34, member_35, member_36,
                   member_37, member_38, member_39, member_40, member_41, member_42, member_43, member_44, member_45,
                   member_46, member_47, member_48, member_49, member_50, member_51, member_52, member_53, member_54,
                   member_55, member_56, member_57, member_58, member_59, member_60, member_61, member_62, member_63);
  }
}
// NOLINTEND
}  // namespace librepr::detail