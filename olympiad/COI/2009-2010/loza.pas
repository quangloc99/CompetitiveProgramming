{**
 * Author: TRan Quang Loc (darkkcyan)
 * Problem: https://wcipeg.com/problem/coi09p3
 * Idea: to calculate the branch, at each node, we have to match the descendants at every level.
 * So for each node we have to maintain 2 list: the left most and the right most node at each level
 * or in my code, I just store the left most and the right most side for simplicity.
 * If we have 2 children, we just need to match the min(left child height, right child height) nodes
 * and then we also merge the lists in the same amount of time, so we end up with complixity O(nlogn).
 * If it is not really clear, the idea is similar to DSU on tree.
 * Please also note for the case we the left most can also be at the right branch and vice versa.
 * See the test case in the end of this file for more intuition.
 * I will not explain how to handle the case here, so please see the code.
 *}
{$mode objfpc}
uses classes, sysutils, math;
const maxn = 300101;

type chain = record
    val: int64;
    lazy: int64;
    next: ^chain;
end;
type PChain = ^chain;

function createChain(val: int64= 0): PChain;
begin 
    result := new(pchain);
    result^.val := val;
    result^.lazy := 0; 
    result^.next := nil;
end;

procedure deleteChain(chain: PChain);
begin 
    if chain = nil then exit;
    deleteChain(chain^.next);
    dispose(chain);
end;

procedure update(var chain: PChain);
begin
    if chain = nil then exit;
    inc(chain^.val, chain^.lazy);
    if chain^.next <> nil then inc(chain^.next^.lazy, chain^.lazy);
    chain^.lazy := 0;
end;

type node = record
    frameLeft, frameRight: int64;
    branch: int64;
    lchild, rchild: int64;
    lchain, rchain: Pchain;
end;

var n: longint;
var nodes: array [0..maxn] of node;

function createNode(nameLength: longint): node;
begin
    inc(nameLength, 2);
    result.frameRight := nameLength div 2;
    result.frameLeft := nameLength - result.frameRight - 1;
    result.branch := 0;
    result.lchild := 0; result.rchild := 0;
    result.lchain := createChain(result.frameLeft);
    result.rchain := createChain(result.frameRight);
end;

procedure addChild(parent: longint; childId: longint);
begin
    if nodes[parent].lchild = 0 then nodes[parent].lchild := childId
    else nodes[parent].rchild := childId;
end;

function calMaxSum(left: Pchain; right: Pchain): int64;
var ans: int64;
begin
    ans := 0;
    while (left <> nil) and (right <> nil) do begin
        update(left);
        update(right);
        ans := max(ans, (left^.val + right^.val));
        left := left^.next;
        right := right^.next;
    end;
    exit(ans);
end;

procedure shorterTail(var u, v: PChain);
begin
    while (u^.next <> nil) and (v^.next <> nil) do begin
        update(u);
        update(v);
        u := u^.next;
        v := v^.next;
    end;
    update(u); update(v);
end;

procedure process;
var u, l, r: longint;
var lc, rc: PChain;
begin
    for u := n downto 1 do begin
        l := nodes[u].lchild; r := nodes[u].rchild;
        if r = 0 then begin
            if l <> 0 then begin
                nodes[u].lchain^.next := nodes[l].lchain;
                nodes[u].rchain^.next := nodes[l].rchain;
            end;
            continue;
        end;

        nodes[u].branch := (calMaxSum(nodes[l].rchain, nodes[r].lchain) + 2) div 2;

        lc := nodes[l].lchain; rc := nodes[r].lchain;
        shorterTail(lc, rc);
        if rc^.next <> nil then begin
            lc^.next := rc^.next;
            rc^.next := nil;
            dec(lc^.next^.lazy, 2 * nodes[u].branch);
        end;


        lc := nodes[l].rchain; rc := nodes[r].rchain;
        shorterTail(lc, rc);
        if lc^.next <> nil then begin
            rc^.next := lc^.next;
            lc^.next := nil;
            dec(rc^.next^.lazy, 2 * nodes[u].branch);
        end;

        nodes[u].lchain^.next := nodes[l].lchain;
        nodes[u].rchain^.next := nodes[r].rchain;
        inc(nodes[l].lchain^.lazy, nodes[u].branch);
        inc(nodes[r].rchain^.lazy, nodes[u].branch);
    end;
end;

function costOf(const n: node): int64;
var res: int64 = 0;
begin
    inc(res, 3 * (n.frameLeft + n.frameRight + 1));
    if n.lchild <> 0 then
        inc(res, 2 + n.branch * 2 + byte(n.lchild <> 0) + byte(n.rchild <> 0));
    exit(res);
end;

var s: string;
var i: longint;
var ans: int64;
var par: longint;
var line: TStringList;

begin
    readln(n);
    readln(s);
    nodes[1] := createNode(length(s));
    for i := 2 to n do begin
        readln(s);
        line := TStringList.create;
        line.delimiter := ' ';
        line.delimitedText := s;
        par := strToInt(line[1]);
        nodes[i] := createNode(length(line[0]));
        addChild(par, i);
        line.Free;
    end;
    process;
    ans := 0;
    for i := 1 to n do inc(ans, costOf(nodes[i]));
    writeln(ans);
end.

{
8
a
b 1
c 1
d 1
e 1
f 4
g 3
abcdefghijklmnopqrst f
}
