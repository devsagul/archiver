import Data,List (nub)

quzNum :: [Char] -> Integer
quzNum q
       | unique == 1 = length q
       | otherwise = 0
       where unique = length . nub . q
