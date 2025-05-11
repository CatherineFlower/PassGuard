#include "cryptomanager.h"
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <cstring>

static void deriveKey(const QString &password, unsigned char *key, unsigned char *iv) {
    QByteArray pw = password.toUtf8();
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(pw.data()), pw.size(), hash);
    memcpy(key, hash, 32);      // 256-bit key
    memcpy(iv, hash + 16, 16);  // 128-bit IV
}

QByteArray CryptoManager::encrypt(const QByteArray &data, const QString &password) {
    unsigned char key[32], iv[16];
    deriveKey(password, key, iv);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    QByteArray encrypted;
    encrypted.resize(data.size() + EVP_MAX_BLOCK_LENGTH);

    int len = 0;
    int totalLen = 0;

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);
    EVP_EncryptUpdate(ctx,
                      reinterpret_cast<unsigned char*>(encrypted.data()), &len,
                      reinterpret_cast<const unsigned char*>(data.data()), data.size());
    totalLen += len;

    EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(encrypted.data()) + totalLen, &len);
    totalLen += len;
    EVP_CIPHER_CTX_free(ctx);

    encrypted.resize(totalLen);
    return encrypted;
}

QByteArray CryptoManager::decrypt(const QByteArray &data, const QString &password) {
    unsigned char key[32], iv[16];
    deriveKey(password, key, iv);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    QByteArray decrypted;
    decrypted.resize(data.size());

    int len = 0;
    int totalLen = 0;

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);
    EVP_DecryptUpdate(ctx,
                      reinterpret_cast<unsigned char*>(decrypted.data()), &len,
                      reinterpret_cast<const unsigned char*>(data.data()), data.size());
    totalLen += len;

    if (!EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(decrypted.data()) + totalLen, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return {}; // ошибка
    }

    totalLen += len;
    EVP_CIPHER_CTX_free(ctx);
    decrypted.resize(totalLen);
    return decrypted;
}
