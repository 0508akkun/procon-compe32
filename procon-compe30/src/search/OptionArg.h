# pragma once
# include <bits/stdc++.h>
# include "cmdline.h"

class OptionArg
{
public:
    /// <summary>
    /// 値を持たないフラグを定義します
    /// </summary>
    static void Register(const std::string& name, char short_name, const std::string& description);

    /// <summary>
    /// 値を持つフラグを定義します
    /// </summary>
    template <typename Type>
    static void Register(const std::string& name,
                  char short_name,
                  const std::string& description,
                  bool need = true,
                  const Type def = Type())
    {
        m_parser.add<Type>(name, short_name, description, need, def);
    }

    /// <summary>
    /// コマンドライン引数を解析します
    /// </summary>
    static void ParseCheck(int argc, char* argv[]);

    /// <summary>
    /// コマンドライン引き数に与えられたフラグが存在していたかを返します
    /// </summary>
    static bool Exist(const std::string& name);

    /// <summary>
    /// コマンドライン引き数に与えられたポート番号を返します
    /// </summary>
    static std::string GetURLInfo();
    
    /// <summary>
    /// 与えられたフラグの値を返します
    /// </summary>
    template <typename Type>
    static const Type& Get(const std::string& name)
    {
        return m_parser.get<Type>(name);
    }

    /// <summary>
    /// usage を返します
    /// </summary>
    static std::string Usage();

private:
    static cmdline::parser m_parser;
};


