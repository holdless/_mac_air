FasdUAS 1.101.10   ��   ��    k             l     ��  ��    9 3 "Macintosh HD:Users:hiroshi:Documents:Applescript"     � 	 	 f   " M a c i n t o s h   H D : U s e r s : h i r o s h i : D o c u m e n t s : A p p l e s c r i p t "   
  
 i         I     �� ��
�� .aevtoappnull  �   � ****  J          ��  o      ���� 	0 _path  ��  ��    Q     S     k    I       r        c        o    ���� 	0 _path    m    ��
�� 
alis  o      ����  0 thecurrentpath theCurrentPath   ��  O   	 I    k    H        r     ! " ! I   �� # $
�� .earslfdrutxt  @    file # o    ����  0 thecurrentpath theCurrentPath $ �� %��
�� 
lfiv % m    ��
�� boovfals��   " l      &���� & o      ���� 0 thefilelist theFileList��  ��      ' ( ' l   ��������  ��  ��   (  ) * ) l   �� + ,��   + S M			set theFileList to name of every file of entire contents of theCurrentPath    , � - - � 	 	 	 s e t   t h e F i l e L i s t   t o   n a m e   o f   e v e r y   f i l e   o f   e n t i r e   c o n t e n t s   o f   t h e C u r r e n t P a t h *  . / . l   ��������  ��  ��   /  0 1 0 l   �� 2 3��   2 ) #			tell application "System Events"    3 � 4 4 F 	 	 	 t e l l   a p p l i c a t i o n   " S y s t e m   E v e n t s " 1  5 6 5 l   �� 7 8��   7 C =				set theNumberOfFiles to (number of files in folder _path)    8 � 9 9 z 	 	 	 	 s e t   t h e N u m b e r O f F i l e s   t o   ( n u m b e r   o f   f i l e s   i n   f o l d e r   _ p a t h ) 6  : ; : l   �� < =��   <  			end tell    = � > >  	 	 	 e n d   t e l l ;  ? @ ? r     A B A l    C���� C n     D E D m    ��
�� 
nmbr E n    F G F 2   ��
�� 
cobj G o    ���� 0 thefilelist theFileList��  ��   B o      ���� $0 thenumberoffiles theNumberOfFiles @  H I H l   �� J K��   J &  			if theNumberOfFiles is 1 then    K � L L @ 	 	 	 i f   t h e N u m b e r O f F i l e s   i s   1   t h e n I  M N M l   �� O P��   O  				return theFileList    P � Q Q , 	 	 	 	 r e t u r n   t h e F i l e L i s t N  R S R l   �� T U��   T  			else    U � V V  	 	 	 e l s e S  W X W r    " Y Z Y m      [ [ � \ \   Z o      ���� 0 thelist theList X  ] ^ ] r   # & _ ` _ m   # $����  ` o      ���� 0 k   ^  a b a U   ' C c d c k   . > e e  f g f r   . 8 h i h b   . 6 j k j b   . 4 l m l o   . /���� 0 thelist theList m l  / 3 n���� n n   / 3 o p o 4   0 3�� q
�� 
cobj q o   1 2���� 0 k   p o   / 0���� 0 thefilelist theFileList��  ��   k m   4 5 r r � s s  
 i o      ���� 0 thelist theList g  t�� t r   9 > u v u [   9 < w x w o   9 :���� 0 k   x m   : ;����  v o      ���� 0 k  ��   d o   * +���� $0 thenumberoffiles theNumberOfFiles b  y z y L   D F { { o   D E���� 0 thelist theList z  | } | l  G G�� ~ ��   ~  				end if     � � �  	 	 	 e n d   i f }  ��� � l  G G��������  ��  ��  ��    m   	 
 � ��                                                                                  MACS  alis    t  Macintosh HD               �<�H+  �f
Finder.app                                                     ������        ����  	                CoreServices    �<q      ��vn    �f�e�d  6Macintosh HD:System: Library: CoreServices: Finder.app   
 F i n d e r . a p p    M a c i n t o s h   H D  &System/Library/CoreServices/Finder.app  / ��  ��    R      �� ���
�� .ascrerr ****      � **** � o      ���� 0 errmsg errMsg��    k   Q S � �  � � � l  Q Q�� � ���   � ) #		display dialog "ERROR: " & errMsg    � � � � F 	 	 d i s p l a y   d i a l o g   " E R R O R :   "   &   e r r M s g �  ��� � L   Q S � � o   Q R���� 0 errmsg errMsg��     ��� � l     ��������  ��  ��  ��       �� � ���   � ��
�� .aevtoappnull  �   � **** � �� ���� � ���
�� .aevtoappnull  �   � ****�� �� ���  �  ���� 	0 _path  ��   � ������ 	0 _path  �� 0 errmsg errMsg � ���� ������������� [���� r����
�� 
alis��  0 thecurrentpath theCurrentPath
�� 
lfiv
�� .earslfdrutxt  @    file�� 0 thefilelist theFileList
�� 
cobj
�� 
nmbr�� $0 thenumberoffiles theNumberOfFiles�� 0 thelist theList�� 0 k  �� 0 errmsg errMsg��  �� T K��&E�O� =��fl E�O��-�,E�O�E�OkE�O �kh����/%�%E�O�kE�[OY��O�OPUW 	X  �ascr  ��ޭ