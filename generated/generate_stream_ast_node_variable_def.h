// ./poof/poof.h:868:0

struct ast_node_variable_def_stream_chunk
{
  ast_node_variable_def Element;
  ast_node_variable_def_stream_chunk* Next;
};

struct ast_node_variable_def_stream
{
  memory_arena *Memory;
  ast_node_variable_def_stream_chunk* FirstChunk;
  ast_node_variable_def_stream_chunk* LastChunk;
  umm ChunkCount;
};

link_internal ast_node_variable_def_stream
AstNodeVariableDefStream(memory_arena *Memory)
{
  ast_node_variable_def_stream Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal void
Deallocate(ast_node_variable_def_stream *Stream)
{
  Stream->LastChunk = 0;
  Stream->FirstChunk = 0;
  VaporizeArena(Stream->Memory);
}

struct ast_node_variable_def_iterator
{
  ast_node_variable_def_stream* Stream;
  ast_node_variable_def_stream_chunk* At;
};

link_internal ast_node_variable_def_iterator
Iterator(ast_node_variable_def_stream* Stream)
{
  ast_node_variable_def_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->FirstChunk
  };
  return Iterator;
}

link_internal b32
IsValid(ast_node_variable_def_iterator* Iter)
{
  b32 Result = Iter->At != 0;
  return Result;
}

link_internal void
Advance(ast_node_variable_def_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}

link_internal b32
IsLastElement(ast_node_variable_def_iterator* Iter)
{
  b32 Result = Iter->At->Next == 0;
  return Result;
}


link_internal ast_node_variable_def *
Push(ast_node_variable_def_stream* Stream, ast_node_variable_def Element)
{
  Assert(Stream->Memory);

  /* (Type.name)_stream_chunk* NextChunk = AllocateProtection((Type.name)_stream_chunk*), Stream->Memory, 1, False) */
  ast_node_variable_def_stream_chunk* NextChunk = (ast_node_variable_def_stream_chunk*)PushStruct(Stream->Memory, sizeof(ast_node_variable_def_stream_chunk), 1, 0);
  NextChunk->Element = Element;

  if (!Stream->FirstChunk)
  {
    Assert(!Stream->LastChunk);
    Stream->FirstChunk = NextChunk;
    Stream->LastChunk = NextChunk;
  }
  else
  {
    Stream->LastChunk->Next = NextChunk;
    Stream->LastChunk = NextChunk;
  }

  Assert(NextChunk->Next == 0);
  Assert(Stream->LastChunk->Next == 0);

  Stream->ChunkCount += 1;

  ast_node_variable_def *Result = &NextChunk->Element;
  return Result;
}


