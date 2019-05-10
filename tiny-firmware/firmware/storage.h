/*
 * This file is part of the Skycoin project, https://skycoin.net/ 
 *
 * Copyright (C) 2014 Pavol Rusnak <stick@satoshilabs.com>
 * Copyright (C) 2018-2019 Skycoin Project
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "types.pb.h"
#include "messages.pb.h"
#include "bip32.h"
#include "tiny-firmware/serialno.h"

#define STORAGE_FIELD(TYPE, NAME) \
    bool has_##NAME; \
    TYPE NAME;

#define STORAGE_STRING(NAME, SIZE) \
    bool has_##NAME; \
    char NAME[SIZE];

#define STORAGE_BYTES(NAME, SIZE) \
    bool has_##NAME; \
    struct { \
        uint32_t size; \
        uint8_t bytes[SIZE]; \
    } NAME;

#define STORAGE_BOOL(NAME)   STORAGE_FIELD(bool,          NAME)
#define STORAGE_NODE(NAME)   STORAGE_FIELD(StorageHDNode, NAME)
#define STORAGE_UINT32(NAME) STORAGE_FIELD(uint32_t,      NAME)

#define DEVICE_LABEL_SIZE 33

typedef struct {
    uint32_t depth;
    uint32_t fingerprint;
    uint32_t child_num;
    struct {
        uint32_t size;
        uint8_t bytes[32];
    } chain_code;

    STORAGE_BYTES(private_key, 32);
    STORAGE_BYTES(public_key,  33);
} StorageHDNode;

typedef struct _Storage {
    uint32_t version;

    STORAGE_NODE   (node)
    STORAGE_STRING (mnemonic, 241)
    STORAGE_BOOL   (passphrase_protection)
    STORAGE_UINT32 (pin_failed_attempts)
    STORAGE_STRING (pin, 10)
    STORAGE_STRING (language, 17)
    STORAGE_STRING (label, DEVICE_LABEL_SIZE)
    STORAGE_BOOL   (imported)
    STORAGE_BYTES  (homescreen, 1024)
    STORAGE_UINT32 (u2f_counter)
    STORAGE_BOOL   (needs_backup)
    STORAGE_UINT32 (flags)
    STORAGE_NODE   (u2froot)
    STORAGE_BOOL   (unfinished_backup)
    STORAGE_UINT32 (auto_lock_delay_ms)
} Storage;

extern Storage storageUpdate;

void storage_init(void);
void storage_generate_uuid(void);
void storage_clear_update(void);
void storage_update(void);
void session_clear(bool clear_pin);

void storage_loadDevice(LoadDevice *msg);

const uint8_t *storage_getSeed(bool usePassphrase);

bool storage_hasLabel(void);
const char *storage_getLabel(void);
const char *storage_getLabelOrDeviceId(void);
void storage_setLabel(const char *label);

const char *storage_getLanguage(void);
void storage_setLanguage(const char *lang);

void storage_setPassphraseProtection(bool passphrase_protection);
bool storage_hasPassphraseProtection(void);

const uint8_t *storage_getHomescreen(void);
void storage_setHomescreen(const uint8_t *data, uint32_t size);

void session_cachePassphrase(const char *passphrase);
bool session_isPassphraseCached(void);
bool session_getState(const uint8_t *salt, uint8_t *state, const char *passphrase);

void storage_setMnemonic(const char *mnemonic);
bool storage_containsMnemonic(const char *mnemonic);
bool storage_hasMnemonic(void);
const char *storage_getMnemonic(void);

bool storage_hasNode(void);

bool storage_containsPin(const char *pin);
bool storage_hasPin(void);
const char *storage_getPin(void);
void storage_setPin(const char *pin);
void session_cachePin(void);
bool session_isPinCached(void);
void storage_clearPinArea(void);
void storage_resetPinFails(uint32_t flash_pinfails);
bool storage_increasePinFails(uint32_t flash_pinfails);
uint32_t storage_getPinWait(uint32_t flash_pinfails);
uint32_t storage_getPinFailsOffset(void);

bool storage_isInitialized(void);

bool storage_isImported(void);
void storage_setImported(bool imported);

bool storage_needsBackup(void);
void storage_setNeedsBackup(bool needs_backup);

void storage_applyFlags(uint32_t flags);
uint32_t storage_getFlags(void);

bool storage_unfinishedBackup(void);
void storage_setUnfinishedBackup(bool unfinished_backup);

void storage_wipe(void);

const char* storage_getFullSeed(void);

/**
 * @brief storage_uuid_str *2 due to the hex format and +1 because of the trailing NULL char
 */
extern char storage_uuid_str[SERIAL_NUMBER_SIZE * 2 + 1];

#endif
