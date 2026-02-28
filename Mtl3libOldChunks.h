
//--------------------------------------------//
// This is old material chunks that were only //
// present back in 10.0~ish when the material //
// file first showed up.                      //
// Moved here to a separate header to keep    //
// thie main template cleaner. But still have //
// it execute these structures if needed      //
//--------------------------------------------//

struct m3MD
{
    ChunkHeader chunk;

    uint64_t qword10;
    uint64_t qword18;
    
    uint32_t unk0;
    uint32_t unk1;
    uint32_t offsetA;
    uint32_t offsetB;
    
    uint32_t offsetC;
    uint32_t offsetF;
    uint32_t unk2;
    uint32_t offsetG;
    
    uint32_t offsetD;
    uint32_t unk4;
    uint32_t offsetH;
    uint32_t offsetE;
    
    // Incorrect ^ v 
    
    //FSeek(chunk.chunkOffset + unk1 * 4);
    //int unks1[unk0]<bgcolor=cLtRed>;
    
    FSeek(chunk.chunkOffset + offsetA * 4);
    uint64_t unksA[unk0]<bgcolor=cGreen, format=hex>;
    
    FSeek(chunk.chunkOffset + offsetB * 4);
    short unksB[unk0]<bgcolor=cYellow>;
    
    FSeek(chunk.chunkOffset + offsetC * 4);
    int unksC[unk0]<bgcolor=cRed>;
    
    FSeek(chunk.chunkOffset + offsetF * 4);
    float unksF[unk0]<bgcolor=cLtGreen>;    // Maybe not an offset
    
    FSeek(chunk.chunkOffset + offsetG * 4);
    uint64_t samplerHashes[unk2]<bgcolor=cLtYellow, format=hex>;
    
    FSeek(chunk.chunkOffset + offsetD * 4);
    int textureFileIDs[unk2]<bgcolor=cLtBlue>;
    
    FSeek(chunk.chunkOffset + offsetH * 4);
    uint64_t unksH[unk4]<bgcolor=cBlue, format=hex>;
    
    FSeek(chunk.chunkOffset + offsetE * 4);
    int unksE[unk4]<bgcolor=cGray>;
    
    /*
    uint32_t offsets0[4];     // 0x20: 4 indices → pointers at a1+16..a1+32
                               //     offsets0[0] → a1+16 (as a 32-bit value)
                               //     offsets0[1] → a1+20
                               //     offsets0[2] → a1+24 (pointer = ptr + 4*offsets0[2])
                               //     offsets0[3] → a1+32 (pointer = ptr + 4*offsets0[3])

    uint32_t offsets1[4];     // 0x30: 4 more indices/values
                               //     offsets1[0] → a1+40 (pointer = ptr + 4*offsets1[0])
                               //     offsets1[1] → a1+48 (pointer = ptr + 4*offsets1[1])
                               //     offsets1[2] → a1+56 (as a 32-bit value)
                               //     offsets1[3] → a1+64 (pointer = ptr + 4*offsets1[3])

    uint32_t offset2;         // 0x40: index → pointer at a1+72
    uint32_t dataValue; // → stored at a1+80
    uint32_t offset3;   // → pointer at a1+88 (ptr + 4*offset3)
    uint32_t offset4;         // 0x4C: index → pointer at a1+96
    */
    
    // This is incomplete, missing data here
    // These are offsets and counts above
    EndChunk(chunk);
};

struct ShaderChunk
{
    struct m3SP
    {
        ChunkHeader chunk;
        int32_t unk0[22];
        int32_t unk1[10];
        EndChunk(chunk);
    };
    
    struct m3ST
    {
        ChunkHeader chunk;
        int32_t unk0[7];
        int16_t unk1[6];
        int32_t unk2[6];
        EndChunk(chunk);
    };
    
    struct m3SS
    {
        ChunkHeader chunk;
        int32_t unk0[7];
        int16_t unk1[10];
        int32_t unk2[14];
        EndChunk(chunk);
    };
  
    ChunkHeader chunk;
    int32_t vertexShaderFDID;
    int32_t pixelShaderFDID;
    int32_t unk0;
    int32_t unk1;
    int32_t unk2;
    int32_t unk3;
    
    while(FTell() < chunk.chunkOffset + 4 * chunk.endOffset)
    {
        if      ( matchr( "m3SP" ) ) m3SP shaderParameters;
        else if ( matchr( "m3ST" ) ) m3ST shaderTextures;
        else if ( matchr( "m3SS" ) ) m3SS shaderSamplers;
        else if ( matchr( "m3MD" ) ) m3MD materialData;
        else SKIPCHUNKM3 unk;
    }
    
    EndChunk(chunk);
};

struct MaterialChunk
{
    ChunkHeader chunk;
    int32_t unk0;
    int32_t unk1;
  
    while(FTell() < chunk.chunkOffset + 4 * chunk.endOffset)
    {
        if      ( matchr( "m3MD" ) ) m3MD materialData;
        else SKIPCHUNKM3 unk;
    }
    
    EndChunk(chunk);
};

struct ShaderLibraryChunk
{
    ChunkHeader chunk;
    int32_t unk0;
    int32_t unk1;
    int32_t unk2;
    uint32_t unk3Hash; // Or not

    while(FTell() < chunk.chunkOffset + 4 * chunk.endOffset)
    {
        if      ( matchr( "m3SH" ) ) ShaderChunk shader;
        else SKIPCHUNKM3 unk;
    }

    EndChunk(chunk);
};

struct MaterialLibraryChunk
{
    ChunkHeader chunk;
    int32_t unk0;
    int32_t unk1;
    int32_t unk2;
    uint32_t unk3Hash;
    
    while(FTell() < chunk.chunkOffset + 4 * chunk.endOffset)
    {
        if      ( matchr( "m3Ml" ) ) MaterialChunk material;
        else SKIPCHUNKM3 unk;
    }
    
    EndChunk(chunk);
};