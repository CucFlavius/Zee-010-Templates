
typedef int8    int8_t;  
typedef int16   int16_t;  
typedef int32   int32_t;  
typedef int64   int64_t;  

typedef uint8   uint8_t;  
typedef uint16  uint16_t;  
typedef uint32  uint32_t;  
typedef uint64  uint64_t;

typedef struct
{
    float x;
    float y;
} vec2 <read=FmtVec2>;

string FmtVec2(vec2 &v)
{
    // adjust %.3f to change precision
    return Str(
        "vec2(%.3f, %.3f)",
        v.x, v.y
    );
}

typedef struct
{
    hfloat x;
    hfloat y;
} vec2h <read=FmtVec2h>;

string FmtVec2h(vec2h &v)
{
    // adjust %.3f to change precision
    return Str(
        "vec2h(%.3f, %.3f)",
        v.x, v.y
    );
}


typedef struct
{
    float x;
    float y;
    float z;
} vec3 <read=FmtVec3>;

string FmtVec3(vec3 &v)
{
    // You can tweak the %.3f to control decimal precision
    return Str(
        "vec3(%.3f, %.3f, %.3f)",
        v.x, v.y, v.z
    );
}

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vec4 <read=FmtVec4>;

string FmtVec4(vec4 &v)
{
    return Str(
        "vec4(%.3f, %.3f, %.3f, %.3f)",
        v.x, v.y, v.z, v.w
    );
}

typedef struct
{
    hfloat x;
    hfloat y;
    hfloat z;
    hfloat w;
} vec4h <read=FmtVec4h>;

string FmtVec4h(vec4h &v)
{
    return Str(
        "vec4h(%.3f, %.3f, %.3f, %.3f)",
        v.x, v.y, v.z, v.w
    );
}

typedef struct
{
    vec4 m0;
    vec4 m1;
    vec4 m2;
    vec4 m3;
} mat4;

typedef struct
{
    local int chunkOffset=FTell();
    
    char ID[4];
    Printf("SkipChunk %s\n", ID);
    int32_t version;
    int32_t endOffset;

    FSeek(chunkOffset + 4 * endOffset);
} SKIPCHUNKM3 <name = SkipChunkName>;

// Helper function for displaying the string instead of the variable name
string SkipChunkName(SKIPCHUNKM3 &str)
{
    if (Strlen(str.ID) > 0)
    {
        return ("Skip : " + str.ID[0] + str.ID[1] + str.ID[2] + str.ID[3]);
    }
    else
    {
        return "";
    }
}

int matchr(char m[])
{
    local char s[] = ReadString( FTell());
    if( m[0] == s[0] && m[1] == s[1] && m[2] == s[2] && m[3] == s[3] ) 
        return 1;
    return 0;
}

typedef struct
{
    char ID[4];
    Printf("SkipChunk Small %s", ID);
    int size;
    if (size > 0)
        byte data[size];
} SKIPCHUNKM3SMALL <name = SkipChunkSmallName>;

// Helper function for displaying the string instead of the variable name
string SkipChunkSmallName(SKIPCHUNKM3SMALL &str)
{
    if (Strlen(str.ID) > 0)
    {
        return ("Skip : " + str.ID[0] + str.ID[1] + str.ID[2] + str.ID[3]);
    }
    else
    {
        return "";
    }
}

struct ChunkHeader
{
    local int chunkOffset = FTell();
    char chunkID[4];
    int32 version;
    int32 endOffset;
    int32 unk1;
};

void EndChunk(ChunkHeader& chunk)
{
    FSeek(chunk.chunkOffset + 4 * chunk.endOffset);
}

struct Uniforms(uint32_t count, uint16_t types[])
{
    local int i;
    for (i = 0; i < count; i++)
    {
        switch(types[i])
        {
            case 0:
                int uniformI;
                break;
            case 14:
                float uniformF;
                break;
            case 15:
                vec2 uniformVec2;
                break;
            case 16:
                vec3 uniformVec3;
                break;
            case 17:
                vec4 uniformVec4;
                break;
            default:
                Printf("UNKNOWN UNIFORM type: %i\n", types[i]);
                break;
        }
    }
};

struct UnkData
{
    uint8_t unk0;
    uint8_t unk1;
    uint16_t unk2;    
};

struct ShaderData(long offset, int version)
{
    uint32_t uniformCount;
    uint32_t uniformDataSize;           // In bytes
    uint32_t uniformHashesOffset;
    uint32_t uniformTypesOffset;
    uint32_t uniformLocationsOffset;
    uint32_t uniformDataOffset;
    
    uint32_t samplerCount;
    uint32_t samplerHashesOffset;
    uint32_t samplerTextureFileIDsOffset;
    
    uint32_t unkDataCount;
    uint32_t unkDataHashesOffset;
    uint32_t unkDataOffset;
    
    if (version == 4)
    {
        // All 0s so far
        uint32_t unk0;
        uint32_t unk1;
    }
    
    local long save = FTell();
    
    if (uniformCount > 0)
    {
        FSeek(offset + uniformHashesOffset * 4);
        unsigned int64 uniformHashes[uniformCount]<bgcolor=cLtGreen, format=hex>;
        FSeek(offset + uniformTypesOffset * 4);
        uint16_t uniformType[uniformCount]<bgcolor=cLtGreen>;
        FSeek(offset + uniformLocationsOffset * 4);
        int uniformLoc[uniformCount]<bgcolor=cLtGreen>;
        FSeek(offset + uniformDataOffset * 4);
        Uniforms uniformData(uniformCount, uniformType);
    }
    
    if (samplerCount > 0)
    {
        FSeek(offset + samplerHashesOffset * 4);
        unsigned int64 samplerHashes[samplerCount]<bgcolor=cLtGreen, format=hex>; // texture hashes ?
        FSeek(offset + samplerTextureFileIDsOffset * 4);
        int samplerTextureFileIDs[samplerCount]<bgcolor=cLtGreen>;
    }
    
    if (unkDataCount > 0)
    {
        FSeek(offset + unkDataHashesOffset * 4);
        unsigned int64 unkDataHashes[unkDataCount]<bgcolor=cLtGreen, format=hex>;
        FSeek(offset + unkDataOffset * 4);
        UnkData unkData[unkDataCount]<bgcolor=cLtGreen>;
    }
    
    FSeek(save); 
};