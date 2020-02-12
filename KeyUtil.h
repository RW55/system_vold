/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_VOLD_KEYUTIL_H
#define ANDROID_VOLD_KEYUTIL_H

#include "KeyBuffer.h"
#include "KeyStorage.h"
#include "Keymaster.h"

#include <fscrypt/fscrypt.h>

#include <memory>
#include <string>

namespace android {
namespace vold {

using namespace android::fscrypt;

bool randomKey(KeyBuffer* key);

bool isFsKeyringSupported(void);

// Install a file-based encryption key to the kernel, for use by encrypted files
// on the specified filesystem using the specified encryption policy version.
//
// For v1 policies, we use FS_IOC_ADD_ENCRYPTION_KEY if the kernel supports it.
// Otherwise we add the key to the legacy global session keyring.
//
// For v2 policies, we always use FS_IOC_ADD_ENCRYPTION_KEY; it's the only way
// the kernel supports.
//
// Returns %true on success, %false on failure.  On success also sets *policy
// to the EncryptionPolicy used to refer to this key.
bool installKey(const std::string& mountpoint, const EncryptionOptions& options,
                const KeyBuffer& key, EncryptionPolicy* policy);

// Evict a file-based encryption key from the kernel.
//
// We use FS_IOC_REMOVE_ENCRYPTION_KEY if the kernel supports it.  Otherwise we
// remove the key from the legacy global session keyring.
//
// In the latter case, the caller is responsible for dropping caches.
bool evictKey(const std::string& mountpoint, const EncryptionPolicy& policy);

bool retrieveKey(bool create_if_absent, const KeyAuthentication& key_authentication,
                 const std::string& key_path, const std::string& tmp_path,
                 bool wrapped_key_supported, KeyBuffer* key, bool keepOld = true);

}  // namespace vold
}  // namespace android

#endif
