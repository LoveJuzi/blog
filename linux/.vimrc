""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => General
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Sets how many lines of history VIM has to remember
if has("win32")
    au GUIEnter * simalt ~x
else
    au GUIEnter * call MaximizeWindow()
endif

function! MaximizeWindow()
    silent !wmctrl -r :ACTIVE: -b add,maximized_vert,maximized_horz
endfunction
set history=1024

" Enable filetype plugin
set nocompatible
filetype plugin on
filetype indent on

" Set to auto read when a file is changed from the outside
set autoread

" With a map leader it's possible to do extra key combinations
" like <leader>w saves the current file
let mapleader = ","
let g:mapleader = ","

" Fast saving
nmap <leader>w :w!<cr>

" Fast editing of the .vimrc
map <leader>e :e! ~/.vim_runtime/vimrc<cr>

" When vimrc is edited, reload it
autocmd! bufwritepost vimrc source ~/.vim_runtime/vimrc

" For tags search scope
set tags=${DEV_ROOT_DIR}/TAGS
"set tags=~/rcehome/CTAGS
set tags+=~/.vim/tags/stl_tags
set tags+=~/.vim/tags/qt_tags
set tags+=~/.vim/tags/ng_tags
set tags+=/pd01/lixq/cascade/occt-879768f/TAGS
set tags+=/rts/builder/devpkgs_new_gcc_rhal5/share/oa-dm4/oa-all-22.43p029_new/oatags
set tags+=/rts/builder/devpkgs_new_gcc_rhal5/share/boost/boost_1_55_0/tags
"$(RCEXPLORER_HOME)/CTAGS
"set autochdir
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => VIM user interface
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Set 7 lines to the curors - when moving vertical..
" set so=5

set wildmenu "Turn on WiLd menu

set ruler "Always show current position

" set cmdheight=1 "The commandbar height

set hid "Change buffer - without saving

" Set backspace config
set backspace=eol,start,indent
set whichwrap+=<,>,h,l

set ignorecase "Ignore case when searching
set smartcase

set hlsearch "Highlight search things

set incsearch "Make search act like search in modern browsers
set nolazyredraw "Don't redraw while executing macros

set magic "Set magic on, for regular expressions

set showmatch "Show matching bracets when text indicator is over them
set mat=2 "How many tenths of a second to blink

" No sound on errors
set noerrorbells
set novisualbell
set t_vb=
set tm=500

"set guifont=Courier\ 14
"set guifont=monospace\ 16
"set guifont=-unknown-DejaVu LGC Sans Mono-normal-normal-normal-*-11-*-*-*-m-0-iso10646-1
set noignorecase
"autocmd GUIEnter * simalt ~x


set ic
set incsearch
set ts=2
set expandtab
set shiftwidth=2
set autoindent
set cindent
set nu!

syntax on

autocmd BufRead * set expandtab
autocmd BufRead Makefile set noexpandtab
autocmd BufRead makefile set noexpandtab
autocmd BufRead GNUmakefile set noexpandtab
autocmd BufRead *target set noexpandtab

command RMC %s!\s*//.*!!g | %s!\s*/\*\_.\{-}\*/\s*!!g
set background=dark
"set nohlsearch
if has("gui_running")
  "colorscheme solarized
  colorscheme evening
  "colorscheme default
  "colorscheme desert
  "colorscheme darkblue
  "colorscheme torte
endif
"colorscheme candy
"colorscheme desert
"colorscheme torte
"colorscheme koehler
"colorscheme murphy
"colorscheme darkblue
"colorscheme blue
"colorscheme evening
"colorscheme pablo

if &term=="xterm"
set t_Co=8
set t_Sb=^[[4%dm
set t_Sf=^[[3%dm
endif

set nocp  
filetype plugin on 

set completeopt=menu
set cindent

let OmniCpp_NamespaceSearch = 1
let OmniCpp_GlobalScopeSearch = 1
let OmniCpp_ShowAccess = 1 
let OmniCpp_ShowPrototypeInAbbr = 1 
let OmniCpp_MayCompleteDot = 1   
let OmniCpp_MayCompleteArrow = 1
let OmniCpp_MayCompleteScope = 1
let OmniCpp_DefaultNamespaces = ["std", "_GLIBCXX_STD"]
au CursorMovedI,InsertLeave * if pumvisible() == 0|silent! pclose|endif 
set completeopt=menuone,menu,longest

"highlight Pmenu    guibg=darkgrey  guifg=black 
"highlight PmenuSel guibg=lightgrey guifg=black

let g:SuperTabDefaultCompletionType="context"

""""""""""""""""""""""""""""""""""""""""""""""""""""
" miniBufExpl
""""""""""""""""""""""""""""""""""""""""""""""""""""
"let g:miniBufExplMapWindowNavVim = 1 
"let g:miniBufExplMapWindowNavArrows = 1 
"let g:miniBufExplMapCTabSwitchBufs = 1 
"let g:miniBufExplModSelTarget = 1 
"nnoremap <C-]> :bn<CR>
"nnoremap <C-[> :bp<CR>

"source ~/.vim/vim_source/vimrc_base
""""""""""""""""""""""""""""""""""""""""""""""""""""
"头文件与源文件相互切换a.vim settings
""""""""""""""""""""""""""""""""""""""""""""""""""""
nnoremap <silent> <F12> :AV<CR> 
"A 	    在新Buffer中切换到c\h文件
"AS 	横向分割窗口并打开c\h文件
"AV 	纵向分割窗口并打开c\h文件
"AT 	新建一个标签页并打开c\h文件

""""""""""""""""""""""""""""""
"Taglist plugin settings 
""""""""""""""""""""""""""""""
"启动vim自动打开taglist
let Tlist_Auto_Open = 0

" 不同时显示多个文件的 tag ，只显示当前文件的
let Tlist_Show_One_File = 1

" 如果 taglist 窗口是最后一个窗口，则退出 vim
let Tlist_Exit_OnlyWindow = 1

"让当前不被编辑的文件的方法列表自动折叠起来 
let Tlist_File_Fold_Auto_Close = 1

"把taglist窗口放在屏幕的右侧，缺省在左侧 
let Tlist_Use_Right_Window=1 

"显示taglist菜单
let Tlist_Show_Menu = 1

"taglist window width
let Tlist_WinWidth = 45
nmap <silent> <F4> :TlistToggle<cr>

"highlight cursor line/column
set cursorcolumn
set cursorline

"""""""""""""""""""""""""""""""""""""""""""""""""""
" hide gvim toolbar
"""""""""""""""""""""""""""""""""""""""""""""""""""
"Toggle Menu and Toolbar
"""set guioptions-=m
"""set guioptions-=T
"""map <silent> <F2> :if &guioptions =~# 'T' <Bar>
"""        \set guioptions-=T <Bar>
"""        \set guioptions-=m <bar>
"""    \else <Bar>
"""        \set guioptions+=T <Bar>
"""        \set guioptions+=m <Bar>
"""    \endif<CR>
"""""""""""""""""""""""""""""""""""""""""""""""""""
" airline 
"""""""""""""""""""""""""""""""""""""""""""""""""""
set t_Co=256
set laststatus=2
" display tabline
let g:airline#extensions#tabline#enabled = 1
" disable trailng
"let g:airline#extensions#whitespace#enable = 0
"let g:airline#extensions#whitespace#symbol = '!'
let g:airline#extensions#tabline#buffer_nr_show = 1
let g:airline#extensions#tabline#fnamemod = ':t'

if !exists('g:airline_symbols')
    let g:airline_symbols = {}
endif
" buffer tab
nnoremap <C-P> :bn<CR>
nnoremap <C-O> :bp<CR>
" Ctrl + h, j, k, l to skip window
"noremap <C-J> <C-W>j
"noremap <C-K> <C-W>k
"noremap <C-H> <C-W>h
"noremap <C-L> <C-W>l

noremap <C-Down>  <C-W>j
noremap <C-Up>    <C-W>k
noremap <C-Left>  <C-W>h
noremap <C-Right> <C-W>l
noremap <C-Tab>   <C-W>r<C-W>w

" set no-highlight shortcut key
map <S-W> :noh<CR>

"""""""""""""""""""""""""""""""""""""""""""""""""""
" indent guides
"""""""""""""""""""""""""""""""""""""""""""""""""""
nmap <silent><F1> <Plug>IndentGuidesToggle
let g:indent_guides_guide_size = 1
let g:indent_guides_start_level = 2
"set cc=80

" add by shengmh 2021-03-16
xnoremap * :<C-u>call <SID>VSetSearch()<CR>/<C-R>=@/<CR><CR>N
xnoremap # :<C-u>call <SID>VSetSearch()<CR>?<C-R>=@/<CR><CR>
function! s:VSetSearch()
  let temp = @s
  norm! gv"sy
  let @/ = '\V' . substitute(escape(@s, '/\'), '\n', '\\n', 'g')
  let @s = temp
endfunction

autocmd BufRead,BufNewFile .redir_0_0 set filetype=cpp
noremap <silent> <C-t> :call CtagJump()<CR>
function CtagJump()
  let filename = expand('%')
  let current_word = expand("<cword>")
  execute "redir! > .redir_0_0 | silent! tag " . current_word . " | redir END | b3 | 1"
  if filename == ".redir_0_0"
    execute "silent! edit!"
  endif
  if line('$') < 5
    execute "b#"
  endif
endfunction

set cursorcolumn!
set cursorline!

let @m='^v$h*'
"map <C-m> :call HighlightLine()<CR>
"function HighlightLine()
"  execute "normal ^v$h"
"  "execute *
"endfunction

" set tab for 4 space
set tabstop=4
set shiftwidth=4
set expandtab
"colorscheme peachpuff
unmap <C-o>
set paste
set nopaste
"set path+=/home/shengmh/src/trunk/SCRIPT/**
nnoremap * *N
set scrolloff=0
set noeb
set laststatus=2
set shortmess+=A
set autoread
"nnoremap <C-w>s :split<CR
"noremap <C-L> zt:split<CR>
"set wildmenu
"set wildmode=longest:list,full
"set backspace=indent,eol,start
"set cursorline
"hi CursorLine term=bold cterm=bold

if &diff
  "colors blue
endif

let Tlist_WinWidth=50

set guioptions-=m
set guioptions-=T

let g:netrw_banner = 0
let g:netrw_liststyle = 1

map <F4> :call SegmentComment()<cr>
map <F3> :call CurrentFilePath()<cr>
map <S-F3> :call CurrentAbsFilePath()<cr>
map <S-F> :wa<cr>:!polas-clang-format -i %:p<cr><cr>
map <S-L> :!clang-format -i %:p<cr>

function SegmentComment()
  call append(line('.')-1, "/************************************************************************************************")
  call append(line('.')-1, "************************ COMMENT                 ************************************************")
  call append(line(".")-1, "************************************************************************************************/")
  echohl WarningMsg | echo "Successful add comment." | echohl None
endfunction

"进行版权声明的设置
"添加或更新头
map <S-F4> :call TitleDet()<cr>
function AddTitle()
  call append(0,"/*************************************************************************")
  call append(1,"*")
  call append(2,"* Author: shengmh")
  call append(3,"*")
  call append(4,"* Create: ".strftime("%Y-%m-%d %H:%M"))
  call append(5,"*")
  call append(6,"* Last modified: ".strftime("%Y-%m-%d %H:%M"))
  call append(7,"*")
  call append(8,"* Filename: ".expand("%:t"))
  call append(9,"*")
  call append(10,"* Description: ")
  call append(11,"*")
  call append(12,"*************************************************************************/")
  echohl WarningMsg | echo "Successful in adding the copyright." | echohl None
endfunction

"更新最近修改时间和文件名
function UpdateTitle()
  normal gg
  execute '/\\* Last modified:/s@:.*$@\=strftime(": %Y-%m-%d %H:%M")@'
  execute '/\\* Filename:/s@:.*$@\=": ".expand("%:t")@'
  echohl WarningMsg | echo "Successful in updating the copy right." | echohl None
endfunction

"判断前10行代码里面，是否有Last modified这个单词，
"如果没有的话，代表没有添加过作者信息，需要新添加；
"如果有的话，那么只需要更新即可
function TitleDet()
  normal my
  let n=1
  "默认为添加
  while n < 10
      let line = getline(n)
      if line =~ '^\* Last\smodified:\S*.*$'
          call UpdateTitle()
          break
      endif
      let n = n + 1
  endwhile
  if n == 10
    call AddTitle()
  endif
  normal `y
endfunction

function CurrentFilePath()
  normal my
  if has('gui_running')
    execute 'let @+=expand("%:.")'
  else
    execute 'let @1=expand("%:.")'
  endif
  echohl WarningMsg | echo "Successful in get current file path." | echohl None
  normal `y
endfunction

function CurrentAbsFilePath()
  normal my
  if has('gui_running')
    execute 'let @+=expand("%:p")'
  else
    execute 'let @1=expand("%:p")'
  endif
  echohl WarningMsg | echo "Successful in get current file path." | echohl None
  normal `y
endfunction

map <C-e> :Ex<cr>
"map <C-d> :bf<cr>
map <C-F5> :redir! > .redir_0_0<cr>
map <F5> :redir END<cr>:b3<cr>:b #<cr>
map gf gF
if has('gui_running') 
  vnoremap <silent> <C-c> "+y
  inoremap <silent> <C-v> <ESC>:set paste<cr>a<C-R>+<ESC>:set nopaste<cr>a
  "noremap <silent> <C-S-V> :set paste<cr>a<C-R>+<ESC>:set nopaste<cr>
  "inoremap <C-S-V> <C-R>+
else
  vnoremap <silent> <C-c> "+y
  inoremap <silent> <C-v> <ESC>:set paste<cr>a<C-R>+<ESC>:set nopaste<cr>a
  "noremap <silent> <C-S-V> :set paste<cr>a<C-R>+<ESC>:set nopaste<cr>
  "inoremap <C-S-V> <C-R>+
endif

set nu!
if has("gui_running")
  "set lines=24
  "set columns=100
endif
"set lines=1000
"set columns=1000
set colorcolumn=90
set noerrorbells
set novisualbell

"set listchars=tab:├─
if has("gui_running")
  "set listchars=tab:_
  "set listchars+=trail:_
  "set list
endif

set guifont=monospace\ 11.8
" set guifont=monospace\ 20 

"set nowrap

" Plugin for Tag List
"filetype plugin on
"let g:Tlist_Show_One_File=1
"let g:Tlist_Exit_OnlyWindow=1
"let g:Tlist_Use_Right_Window=1
"map <F8> :TlistToggle<CR>

"let g:Tlist_UsePython=1

" when we load perl syntax it will add : in keyword and fname
" we remove the syntax from perl filtype
autocmd FileType perl set iskeyword-=:
autocmd FileType perl set isfname-=:

" it is a dangerous option, it means we do not generate temp file
set noswapfile

set vb t_vb=

"set winwidth=200
"set winheight=200

" disable Omni Completion
set omnifunc=""
let OmniCpp_MayCompleteDot = 0
let OmniCpp_MayCompleteArrow = 0
let OmniCpp_MayCompleteScope = 0

noremap <C-m> :set columns=210<CR>:set lines=53<CR>

" 开启行号显示
" set number

" 启用语法高亮
syntax enable
set background=dark

" 设置自动缩进
set autoindent
set smartindent
set tabstop=4
set shiftwidth=4
set expandtab

" 显示匹配的括号
set showmatch

" 启用行尾空格和 TAB 符号高亮
highlight ExtraWhitespace ctermbg=red guibg=red
match ExtraWhitespace /\s\+$\| \+\ze\t/

" 启用代码折叠
" set foldmethod=indent
" set foldlevel=99

" 启用搜索时忽略大小写
set ignorecase
set smartcase

" 启用实时搜索
set incsearch
set hlsearch

" 启用文件类型检测
filetype plugin indent on

" 设置显示行号的宽度
set numberwidth=5

" 启用状态行显示文件类型
set ruler
set laststatus=2

" 启用自动补全
set completeopt=menuone,noselect
set shortmess+=c

" Enable YCM
let g:loaded_ycm = 1

""""""""""""""""""""coc.vim begin""""""""""""""""""""""""""""""""""""""""""""""
" 启用 coc.nvim
let g:coc_global_extensions = ['coc-snippets', 'coc-pyright', 'coc-clangd']

" 配置 Python 语言服务器（pyright）
autocmd FileType python setl omnifunc=python3complete#Complete
let g:coc_server_pyright_use_pyls = 0

autocmd FileType python map <S-f> :%!autopep8 -<cr>

" 配置键盘快捷键
nmap <silent> gd <Plug>(coc-definition)
nmap <silent> gy <Plug>(coc-type-definition)
nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)
" 其他键盘映射
nnoremap <leader>rn <Plug>(coc-rename)
""""""""""""""""""""coc.vim end""""""""""""""""""""""""""""""""""""""""""""""""

call plug#begin('~/.vim/plugged')
Plug 'vim-python/python-syntax'
Plug 'ycm-core/YouCompleteMe'
Plug 'neomake/neomake'
Plug 'neoclide/coc.nvim', {'branch': 'release'}
call plug#end()

