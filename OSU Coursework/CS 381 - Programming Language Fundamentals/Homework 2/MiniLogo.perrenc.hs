module MiniLogo where

import Data.List

--
-- * MiniLogo
--
-- | The grammar:
--      num ::= (any natural number)
--      var ::= (any variable name)
--    macro ::= (any macro name)
--
--     prog ::= ε | cmd; prog                 sequence of commands
--
--     mode ::= up | down                     pen status
--
--     expr ::= var                           variable reference
--           |  num                           literal number
--           |  expr + expr                   addition expression
--
--      cmd ::= pen mode                      change pen status
--           |  move (expr, expr)             move pen to a new position
--           |  define macro (var*) {prog}    define a macro
--           |  call macro (expr*)            invoke a macro

-- | 1. Define the abstract syntax as a set of Haskell data types.
type Var = String
type Macro = String

type Prog = [Cmd]

data Mode = Up
          | Down
          deriving (Show, Eq)

data Expr = VarRef Var
          | LitNum Int
          | Add Expr Expr
          deriving (Show, Eq)

data Cmd = Pen Mode
         | Move (Expr, Expr)
         | Define Macro [Var] Prog
         | Call Macro [Expr]
         deriving (Show, Eq)


-- | 2. Define a MiniLogo macro "line."
--
--      Concrete syntax in a comment:
--
--      define line(x1, y1, x2, y2) {
--          pen up;
--          move (x1, y1);
--          pen down;
--          move (x2, y2);
--          pen up;
--      }
--
--      Abstract syntax in code (include correct type header):
--
line :: Cmd
line = Define "line" ["x1", "y1", "x2", "y2"] [Pen Up, Move (VarRef "x1", VarRef "y1"), Pen Down, Move (VarRef "x2", VarRef "y2"), Pen Up]

-- | 3. Define a MiniLogo macro "nix" using "line" defined above.
--
--      Concrete syntax in a comment:
--
--      define nix(x, y, w, h) {
--          line(x, y, x + w, y + h);
--          line(x + w, y, x, y + h);
--      }
--
--      Abstract syntax in code (include correct type header):
--
nix :: Cmd
nix = Define "nix" ["x", "y", "w", "h"]
                   [Call "line" [VarRef "x", VarRef "y", Add (VarRef "x") (VarRef "w"), Add (VarRef "y") (VarRef "h")],
                    Call "line" [Add (VarRef "x") (VarRef "w"), VarRef "y", VarRef "x", Add (VarRef "y") (VarRef "h")]]


-- | 4. Define a Haskell function "steps" (steps :: Int -> Prog) that draws
--      a staircase of n steps starting from (0,0).
--
steps :: Int -> Prog
steps 0 = []
steps i = [Call "line" [LitNum i, LitNum i, LitNum (i - 1), LitNum i], Call "line" [LitNum i, LitNum i, LitNum i, LitNum (i - 1)]] ++ steps (i-1)


-- | 5. Define a Haskell function "macros" (macros :: Prog -> [Macro] that
--      returns a list of the names of all the macros that are defined anywhere
--      in a given MiniLogo program.
--
macros :: Prog -> [Macro]
macros [] = []
macros (h:t) = case h of
             Define m _ _ -> m : macros t
             otherwise -> macros t  


-- | 6. Define a Haskell function "pretty" (pretty :: Prog -> String) that
--      "pretty-prints" a MiniLogo program.
--
pretty :: Prog -> String
pretty [] = ""
pretty (Pen Up:t) = "pen up;\n" ++ pretty t
pretty (Pen Down:t) = "pen down;\n" ++ pretty t
pretty (Move (x, y):t) = "move (" ++ exprString x ++ ", " ++ exprString y ++ ");\n" ++ pretty t
pretty (Call n h:t) = n ++ "(" ++ intercalate ", " (map exprString h) ++ ");\n" ++ pretty t
pretty (Define m h p:ps) = "define " ++ m ++ "(" ++ intercalate ", " h ++ ") {\n" ++ pretty p ++ "}\n" ++ pretty ps

exprString :: Expr -> String
exprString (Add x y) = exprString x ++ " + " ++ exprString y
exprString (LitNum n) = show n
exprString (VarRef s) = s

--
-- * Bonus Problems
--
-- | 7. Define a Haskell function "optE" (optE :: Expr -> Expr) that partially
--      evaluates expressions by replacing additions of literals with the
--      result.
--
optE = undefined


-- | 8. Define a Haskell function "optP" (optP :: Prog -> Prog) that optimizes
--      all of the expressions contained in a given program using optE.
--
optP = undefined
