{**
 * Author: Tran Quang Loc (darkkcyan)
 * Editorial: https://www.hackerearth.com/ru/problem/algorithm/make-n00b_land-great-again-circuits/editorial/
 *}

type llist = record
    v: longint;
    nxt: ^llist;
end;
type pllist = ^llist;
 
procedure push(var l: pllist; x: longint);
var t: pllist;
begin  new(t); t^.v := x; t^.nxt := l; l := t; end;
 
function pop(var l: pllist): longint;
var t: pllist;
begin pop := l^.v; t := l^.nxt; dispose(l); l := t; end;
 
function next(var l: pllist): longint;
begin next := l^.v; l := l^.nxt; end;
 
const maxn = 500100;
var
    n, m, q: longint;
    gr, st: array[0..maxn] of pllist;
    f, x, d: array[0..maxn] of longint;
    th: array[0..maxn] of int64;
    
procedure enter;
var i, u: longint;
begin
    read(n, m);
    fillchar(gr, sizeof(gr), 0);
    fillchar(st, sizeof(st), 0);
    for i := 2 to n do begin
        read(u);
        push(gr[u], i);
    end;
    for i := 1 to n do begin
        read(u);
        push(st[u], i);
    end;
    for i := 1 to m do read(th[i]);
    read(q);
    for i := 1 to q do begin
        read(f[i], x[i], d[i]);
    end;
end;
 
function get(var bit: array of int64; i: longint): int64;
var ans: int64 = 0;
begin
    inc(i); while i > 0 do begin
        inc(ans, bit[i]);
        dec(i, i and -i);
    end;
    exit(ans);
end;
 
procedure upd(var bit: array of int64; i: longint; v: int64);
begin
    inc(i); while i < length(bit) do begin
        inc(bit[i], v);
        inc(i, i and -i);
    end;
end;
 
var
    bb, ee, de: array[0..maxn] of longint;
    cur: longint;
    
procedure dfs(u, p: longint);
var i: pllist; v: longint;
begin
    de[u] := de[p] + 1;
    bb[u] := cur; inc(cur);
    i := gr[u];
    while i <> nil do begin
        v := next(i);
        if v = p then continue;
        dfs(v, u);
    end;
    ee[u] := cur;
end;
 
var h, c1: array [0..maxn] of int64;
procedure apply(qid: longint); inline;
var cost1: int64;
begin
    cost1 := x[qid] - int64(d[qid]) * de[f[qid]];
    upd(h, bb[f[qid]], cost1);
    upd(h, ee[f[qid]], -cost1);
    upd(c1, bb[f[qid]], d[qid]);
    upd(c1, ee[f[qid]], -d[qid]);
    // upd(c2, bb[f[qid]], d[qid] * de[f[qid]]);
    // upd(c2, ee[f[qid]], -d[qid] * de[f[qid]]);
end;
 
var
    l, r: array[0..maxn] of longint;
    cl: array[0..maxn] of pllist;
    
function cal(u: longint): int64; inline;
// var m, n, p: int64;
begin
    // m := get(h, bb[u]);
    // n := de[u] * get(c1, bb[u]);
    // p := get(c2, bb[u]);
    // write('(', m, ' ', n, ' ', p, ')');
    exit(get(h, bb[u]) + de[u] * get(c1, bb[u]));
end;
    
function ok(u: longint): boolean; inline;
var
    s: int64 = 0;
    v: longint;
    itor: pllist;
begin
    itor := st[u];
    // write(u, '(');
    while itor <> nil do begin
        v := next(itor);
        inc(s, cal(v));
        // write(':', v, ' ');
        if s >= th[u] then exit(true);
    end;
    // writeln(') ', s);
    exit(false);
end;
 
procedure process;
var
    i, f, u, v, t: longint;
    haschange: boolean;
begin
    for i := 1 to m do begin
        l[i] := 1;
        r[i] := q + 1;
    end;
    fillchar(cl, sizeof(cl), 0);
    haschange := true;
    while haschange do begin
        for i := 1 to m do
            if l[i] <> r[i] then begin
                push(cl[l[i] + (r[i] - l[i]) div 2], i);
            end;
        fillchar(h, sizeof(h), 0);
        fillchar(c1, sizeof(c1), 0);
        // fillchar(c2, sizeof(c2), 0);
        haschange := false;
        for i := 1 to q do begin
            apply(i);
            while cl[i] <> nil do begin
                u := pop(cl[i]);
                // write(i, ' ');
                if ok(u) then r[u] := i
                else l[u] := i + 1;
                haschange := haschange or (l[u] <> r[u]);
            end;
        end;
    end;
end;
 
var i: longint;
begin
    enter;
    de[0] := -1;
    dfs(1, 0);
    // for i := 1 to n do writeln(i, ' ', bb[i], ' ', ee[i], ' ', de[i]);
    process;
    for i := 1 to m do begin
        if l[i] = q + 1 then writeln('rekt')
        else writeln(l[i]);
    end;
end.
