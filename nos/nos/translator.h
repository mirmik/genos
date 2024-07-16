#ifndef NOS_TRANSLATOR_H
#define NOS_TRANSLATOR_H

#include <memory>
#include <string>
#include <unordered_map>

namespace nos
{
    class ITranslate
    {
    public:
        virtual ~ITranslate() = default;
        virtual std::string translate(const std::string &text) = 0;
    };

    class Translate : public ITranslate
    {
        std::unordered_map<std::string, std::string> translations;

    public:
        Translate(
            const std::unordered_map<std::string, std::string> &translations)
            : translations(translations)
        {
        }

        std::string translate(const std::string &text) override
        {
            auto it = translations.find(text);
            if (it != translations.end())
            {
                return it->second;
            }
            return text;
        }
    };

    class Translator
    {
        std::unordered_map<std::string, std::unique_ptr<ITranslate>> translates;
        std::string currentLanguage;

    public:
        void addTranslate(const std::string &language,
                          std::unique_ptr<ITranslate> translator)
        {
            translates[language] = std::move(translator);
        }

        void setLanguage(const std::string &language)
        {
            currentLanguage = language;
        }

        std::string translate(const std::string &text)
        {
            auto it = translates.find(currentLanguage);
            if (it != translates.end())
            {
                return it->second->translate(text);
            }
            return text;
        }

        static Translator &instance()
        {
            static Translator instance;
            return instance;
        }
    };
}

#endif