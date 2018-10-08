#pragma once

enum CaseTypeOption : int
{
    camel,
    hungarian,
    pascal,
    snake
};

enum class CaseType : int
{
    Camel     = camel,
    Hungarian = hungarian,
    Pascal    = pascal,
    Snake     = snake,
    Unknown
};
