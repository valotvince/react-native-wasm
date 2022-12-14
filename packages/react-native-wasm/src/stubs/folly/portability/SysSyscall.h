/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#if !defined(_WIN32) && !defined(__XROS__)
#include <sys/syscall.h>

#if defined(__APPLE__)
#define FOLLY_SYS_gettid SYS_thread_selfid
#elif defined(SYS_gettid)
#define FOLLY_SYS_gettid SYS_gettid
#else
#define FOLLY_SYS_gettid pthread_self
#endif
#endif
