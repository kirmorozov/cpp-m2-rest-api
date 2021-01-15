#include <sodium.h>
#include <random>

#ifndef MG_API_ENCRYPTOR_H
#define MG_API_ENCRYPTOR_H

namespace mg::m2 {
    class Encryptor {
    public:
        /**
         * Key of md5 algorithm
         */
        const int HASH_VERSION_MD5 = 0;

        /**
         * Key of sha256 algorithm
         */
        const int HASH_VERSION_SHA256 = 1;

        /**
         * Key of Argon2ID13 algorithm
         */
        const int HASH_VERSION_ARGON2ID13 = 2;

        /**
         * Key of latest used algorithm
         * @deprecated
         * @see \Magento\Framework\Encryption\Encryptor::getLatestHashVersion
         */
        const int HASH_VERSION_LATEST = 2;

        /**
         * Default length of salt in bytes
         */
        const int DEFAULT_SALT_LENGTH = 32;

        /**#@+
         * Exploded password hash keys
         */
        const int PASSWORD_HASH = 0;
        const int PASSWORD_SALT = 1;
        const int PASSWORD_VERSION = 2;
        /**#@-*/

        /**
         * Array key of encryption key in deployment config
         */
        const std::string PARAM_CRYPT_KEY = "crypt/key";

        /**#@+
         * Cipher versions
         */
        const int CIPHER_BLOWFISH = 0;

        const int CIPHER_RIJNDAEL_128 = 1;

        const int CIPHER_RIJNDAEL_256 = 2;

        const int CIPHER_AEAD_CHACHA20POLY1305 = 3;

        const int CIPHER_LATEST = 3;
        /**#@-*/

        /**
         * Default hash string delimiter
         */
        const std::string DELIMITER = ":";

        std::string _keys = "";

        Encryptor() {
            _keys = "";
        }

        Encryptor(std::string encryption_keys) {
            _keys = encryption_keys;
            if (sodium_init() < 0) {

                throw "Cant load sodium!";
                /* panic! the library couldn't be initialized, it is not safe to use */
            }
        }

        bool validateHash(std::string password, std::string hash) {
            return isValidHash(std::move(password), std::move(hash));
        }

        bool isValidHash(std::string password, std::string hash) {
            std::string recreated = "";
            std::string clean_hash = "";
            try { /* */
                if (hash.substr(hash.length() - 2, 2) == ":2") {
                    clean_hash = hash.substr(0, hash.find(':'));
                    auto salt = hash.substr(hash.find(':') + 1, hash.length() - clean_hash.length() - 3);
                    recreated = _hash_argon(password, salt);
                } else if (hash.substr(hash.length() - 2, 2) == ":1") {
                    clean_hash = hash.substr(0, hash.find(':'));
                    auto salt = hash.substr(hash.find(':') + 1, hash.size() - clean_hash.size() - 3);
                    recreated = _hash_sha256(password, salt);
                } else if (hash.substr(hash.length() - 2, 2) == ":0") {
                    clean_hash = hash.substr(0, hash.find(':'));
                    auto salt = hash.substr(hash.find(':') + 1, hash.length() - clean_hash.length() - 3);
                    recreated = _hash_md5(password, salt);
                } else {
                    recreated = "bad";
                    clean_hash = "good";
                }


            } catch (const std::exception &e) {
                /* */
                recreated = _hash(password);
            }

            return recreated == clean_hash;
        }

        std::string _hash(std::string password) {
            return "123123qa";
        }

        std::string random_string(std::size_t length)
        {
            const std::string characters = "0123456789abcdefghijklmnopqrstuvwxyz";

            std::random_device random_device;
            std::mt19937 generator(random_device());
            std::uniform_int_distribution<> distribution(0, characters.size() - 1);

            std::string random_string;

            for (std::size_t i = 0; i < length; ++i)
            {
                random_string += characters[distribution(generator)];
            }

            return random_string;
        }

    protected:

        std::string _hash_md5(const std::string &password, std::string salt) {

            unsigned char out[crypto_hash_sha256_BYTES];

            std::string message = salt + password;
            const char* c_message = message.c_str();
            unsigned char* uc_message = *(unsigned char**)(&c_message);
            if (crypto_hash_sha256(out, uc_message, message.length())!=0)
            {
                throw "Sodium cant hash!";
            }

            std::string res = bin2hex(out, crypto_hash_sha256_BYTES);
            //res.append((char *)out);
            return res;
        }

        std::string _hash_sha256(const std::string &password, std::string salt) {

            unsigned char out[crypto_hash_sha256_BYTES];

            std::string message = salt + password;
            const char* c_message = message.c_str();
            unsigned char* uc_message = *(unsigned char**)(&c_message);
            if (crypto_hash_sha256(out, uc_message, message.length())!=0)
            {
                throw "Sodium cant hash!";
            }

            std::string res = bin2hex(out, crypto_hash_sha256_BYTES);
            //res.append((char *)out);
            return res;
        }

        std::string _hash_argon(const std::string &password, std::string salt) {
            int pwlength = 32, hash_len = 32U; crypto_pwhash_STRBYTES;
                                        //crypto_pwhash_STRBYTES;

            //unsigned char slt[16];
            const char* c_salt = salt.c_str();
            unsigned char hashed_password[hash_len];
            unsigned char* slt = *(unsigned char**)(&c_salt);

            if (crypto_pwhash(
                    hashed_password,
                    hash_len,
                    password.c_str(), password.length(),
                    slt,
                    crypto_pwhash_opslimit_interactive(),
                    crypto_pwhash_memlimit_interactive(),
                    crypto_pwhash_alg_argon2id13()) != 0) {
                /* out of memory */
                throw "Sodium out of memory!";
            }

            return bin2hex(hashed_password, hash_len);
        }

        std::string bin2hex(const unsigned char * data, int len) {
            constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                       '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
            std::string res(len * 2, ' ');
            for (int i = 0; i < len; ++i) {
                res[2 * i]     = hexmap[(data[i] & 0xF0) >> 4];
                res[2 * i + 1] = hexmap[data[i] & 0x0F];
            }


            return res;
        }
    };

}


#endif