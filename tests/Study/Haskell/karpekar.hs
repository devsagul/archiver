import Data.List (sort)

karpekar :: Int -> Int
karpekar a
         | a == 6174 = 0
         | otherwise = 1 + karpekar (acsending - descending)
         where
           arr = sort $ (show a)
           acsending = read arr :: Int
	   reversed = reverse arr
           descending = read reversed :: Int
