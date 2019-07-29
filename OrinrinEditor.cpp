/*! @file
	@brief �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
	���̃t�@�C���� OrinrinEditor.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/05/10
*/

/*
Orinrin Editor : AsciiArt Story Editor for Japanese Only
Copyright (C) 2011 - 2014 Orinrin/SikigamiHNQ

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/
//	����{�鍑���p��́�������ꂽ��
//-------------------------------------------------------------------------------------------------

//	���ӁE�R�}���h�̃��\�[�X�h�c�ԍ��͕ύX�s�I


//	TODO:	BIG_TEXT_SEPARATE	�ŋ�؂�̂Ȃ�TXT���ǂ������m�F����E���������Ȃ�K���ȂƂ���ŕ�����
//			AST�łȂ��Ȃ���e�m�F���āA��؂肪���邩�ǂ����m�F�B�P�O�O�s�ȏ゠��A����؂肪�Ȃ��Ȃ�A����e�L�X�g�Ƃ݂Ȃ�
//			���̏ꍇ�́A����t�@�C���Ȃ̂ŕ����Ǎ����邩�A��߂邩�ɂ���B
//			�������ɁA���������������ɂ��ăt�@�C�������āA�����ɏo�͂������āA������J�������H
//			�S�O�s�ȏォ�S�s�ȏ�̋󔒉��s������΁A�����ŕ�������





//	TODO:	�J�[�\���ʒu���獶�E�����ւ���B���E�z�u�Ɏg���B
//���s�݂̂̍s�ň͂܂ꂽ�͈͂�ΏہB

//	TODO:	�n�j�H	�ۑ������Ƃ��̃o���[�����b�Z�[�W��ON/OFF�ł���悤��
//	TODO:	�n�j�H	MLT�̕��^�u�A���i�ƃX�N���[���A�I���o����悤��
//	TODO:	�n�j�H	MLT�̕��^�u�A�_�u���N���b�N�Ŗ��̕ύX���o����悤��

//	TODO:	�n�j�H	MAA�t�@�C���̃R�}�̍폜��������Ă�	MaaItems.cpp
//	TODO:	�n�j�H	MAA�\�����̃��c���當���񌟍�����@�\
//	TODO:	�n�j�H	�h���t�g�E�T���l�̃T�C�Y�ύX�@�\
//	TODO:	�n�j�H	�ő剻�������ʃT�C�Y������ƁA�E�̕ňꗗ�������Ȃ��Ȃ�
//	TODO:	�n�j�H	�ő剻���ďI��遄�N�����āA�ő剻��������ƁA���C���X�v���b�g�o�[�������Ȃ��Ȃ�//�E�ɃC�b������Ă�܂�

//	TODO:	�n�j�H	�ʒu���̃��Z�b�g�@�\�E�V�X�e�����j���[�ɓ��ڂ���
//	TODO:	�n�j�H	���C���X�v���b�g�o�[�̈ʒu��񃊃Z�b�g�����邩��

//	TODO:	�n�j�H	�{���̓r�����獶�E�ɂ��炷�@�\�B�L�����b�g�ʒu����ɁA��������E���A���E�ɂ��炷�B
//			�������͑I��͈͂̂݁B�J�n�ʒu�ɒ���
//			�܂�Ȃ��̋󔒂�L�яk�݂����銴����
//	#ifdef DOT_SPLIT_MODE


//	TODO:	�n�j�H	MAA���ŁA���̗��ŉE�N���b�N���u�����ɃA�C�e����ǉ��v��AA��ǉ������ہA
//			����mlt�̑�AA�S�Ă̖����ɉ��s����������ɒǉ������


//�{�������A�e�R�ŕ\���X�V�H


//	TODO:	�N�����ɊO������̃R�}���h�ł�����t�@�C�����J����悤�ɂł��Ȃ��H
//�v���Z�X������Ƃ��Ɂu"�`\OrinrinEditor.exe" "�`.mlt"�v�Ƃ����R�}���h���C�������s������
//�G���[���o�āA���������ꍇ�ɋN�����̕ʃ^�u�ɓǂݍ��ނ��A������ʂɗ����オ���ė~����
//���̎d�l���ƁA�֘A�t���ŃG�f�B�^�[�Ƃ��ēo�^���Ă��t�@�C������͈�����J���Ȃ����A
//Ayaya Viewer��MLTView�ŊO���ҏW�v���O�����Ƃ��ēo�^���Ă��N�������Ǝg�p�ł��Ȃ��̂ŁA
//�����pAST�̕ҏW�ƍ�i�p�����܂Ƃ�MLT�̃����e�i���X���s�ւł��̂ŁB

//�m�[�g�i1280*800�j�ƊO�t���i1920*1080�j��2��ʍ\���Ńm�[�g��
//���C���f�B�X�v���C�ɂ����Ƃ��T�u�f�B�X�v���C�ŕ���AA�ϊ��{�b�N�X�A
//�c�����ϊ��{�b�N�X��\������ƃ��C���f�B�X�v���C���̈ړ��ł��Ȃ��ʒu��
//�{�b�N�X���\������g���Ȃ��Ȃ�܂��B

//����ƃG�f�B�^�̉��������߂����Ƀu���V�Ȃǂ��\������Ă���p���b�g��
//�ړ����ꂸ�����Ă��܂��̂ł�������\�����ꑱ����ݒ肪�ł���悤�ɂ��肢���܂�

//MonitorFromPoint
//multimon.h
//#define COMPILE_MULTIMON_STUBS
//GetSystemMetrics(SM_CMONITORS)


//�������̂Ń`�F�L����
//	TODO:	�ǉ������y�[�W�Z�p���[�^�[���u[AA][[AA][<�ǉ����ɓ��͂����y�[�W�^�C�g��>]<���s>]�v���ĂȂ�݂����B
//	TODO:	�Ő؂�ւ������A�Ō�̕ł̂Ƃ���܂ŕňꗗ���X�N���[��


//	TODO:	�f�B���C���[�h�ECtrl Shift D �̓���͖�薳����

//	TODO:	�ۑ����̊g���q�w��I���E�I�[�g�AMLT�AAST��TXT���g��Ȃ���΂���ł�������
//	TODO:	�S�����̕����Ɍv�Z���Ă����Α����������������ĉe���Ȃ�������

//	TODO:	�ŏ������畜�A����ƁAMAA��\���ł��\���ɂȂ����Ⴄ
//	���C������WM_ACTIVATE�ɗ���O�ɕ\������Ă�HMAA����ACTIVATE��߂܂���K�v�����邩
//	�Ȃ�����u������B�ǂ̃^�C�~���O���H

//	TODO:	�ėp�`�`�f�B���N�g���ɃW�����v�ł���悤�ɁA�Ƃ��E�u�b�N�}�[�N�@�\�H

//	TODO:	�E�������̕����ύX�ł��Ȃ��H

//	TODO:	�l�`�`�e���v�������V���[�g�J�b�g���Ƃ�

//	TODO:	viewer��MLT���X�g�A�b�v�A�`�F�b�N�{�b�N�X�͊D�F�ł��Ȃ���
//			�f�B���N�g�����̑I���I������₱����
//			���[�g�f�B���N�g���̍X�V�������݂āA�C�����V�K�����f�H

//	TODO:	MAA�̂`�`�\���͍���������

//	TODO:	���j���[�̃w���v����A������AST�����ł��Ăׂ�悤�ɂȂ��Ă�Ƃ���

//	TODO:	�ŏI�s�̘g�����Ɨ�����H




//	TODO:	���C���̃t�@�C���^�u�ɁA���̃t�@�C���ӊO�����@�\������
//	TODO:	�ňꗗ�A�I�����ړ�������\���ł��ǐ����遃�����I�������Ă���
//	TODO:	�S�s�����h����������@�\�E�s�����h�̑O����m�F���āA�󔒂ł���Ε��ɍ��킹�Ēu�������E�P�ƂȂ�3dot���j�R�[�h�Œu��������

//	TODO:	MAA�̈�R�}�̕ҏW�@�\�E�t�@�C�����H�v���邩�t���O�ŊǗ�
//			�ۑ�����ɂ́A����MAA�̊J�����ێ����Ă����K�v������

//	TODO:	�y�[�W���X�g���A�΂炵���Ƃ�������悤��

//	TODO:	�}���ɂ���F�w����J�X�^�}�C�Y�o����


//�l����

//	���C���[�@�\�Ƃ́H

//	�ō폜���b�Z�[�W�̊m�F�����́A�N���������ɂ����ق��������H
//	�L�[�o�C���h�E���j���[�ɓ����ĂȂ��@�\�ɒ���
//	�g�ҏWDIALOGUE�A�I�t�Z�b�g��ENTER�Ƃ��������ɑΉ��Z��
//	�h�b�L���O�e���v���A���ɒu���Ȃ���
//	�Z�[�u�����b�Z�[�WON/OFF�ł���悤��
//	�t�@�C�������́A�����������ԂɂȂ�悤�ɂ��ׂ����n�j�H
//	�v���r���[�̍X�V�{�^������
//	�v���r���[�J���Ă鎞�ɁA�J�����삵�������H

//	�f�J���t�@�C���J�����Ƃ����Ƃ��̓��b�Z�[�W�o���Ƃ�
//	SQL��INSERT�Aprepare�͏d���B�N�G�������āAreset���Ȃ���񂷂̂������񂶂�
//	IE�v���r���[�ACSS�Ńt�H���g�\���𐳋K������ http://b4t.jp/823



//�t�@�C���J���Ƃ��A�y�[�W�f�[�^�͐��̃e�L�X�g�f�[�^�����Q�b�g���Ă����āA�������W�J�͂��Ȃ�
//�y�[�W���I�����ꂽ��A�{�̂ɓW�J���āA���f�[�^�͏����Ă����E������t���O�ɂ���
//�ۑ�����Ƃ��́A���f�[�^���A�{�̃f�[�^��ۑ�����
//�ŕ\���̃X�e�[�^�X�̂ق��́H�o�C�g���v�Z�����Ȃ炻��Ȃɏd���Ȃ��H

//�Ŕԍ��}���݂����ɁA�����łɂ킽�鏈�������o�C
//�S�v���r���[�����o�C

//	�t�@�C���I�[�|�����Ƀv���O���X�I�Ȃ��̂��B�o�C�g���\���̂�B�f�B���C���[�h�ł���Ηv��Ȃ���


//�m�F�FViewer���A����PageUpDown�����Ă邩

//	�t�@�C����ǂݍ��ނƃ����^�C���G���[�ł邱�Ƃ�����E�Ȃ񂩕ύX���Ă���H
//	�X�N���[�������������ꍇ�A��ʑS�̏����K�v��

//�e���Ȃ��悤�ɂ���
//�I��͈͂������Ԃ�CtrlU���A���h�D���I��͈͂����������Ȃ�B����ɐ؂���Ƃ�����Ɨ�����
//�A���h�D���h�D������A�I��͈͉�������EDocInsDelCtrl.cpp:559������ł��������H

//�񃆃j�R�[�h�̖��߃p�^�[���A���p�󔒋C�ɂ��Ȃ��p�ɂ���H��������ƕύX���Ă݂�




//�o�O

//	TODO:	��`�I��͈̓o�C�g�J�E���g���x��
//	TODO:	�`��X�V�^�C�~���O�����������ꍇ�����X����
//	TODO:	�u��`�I���v�Ɓu�������o�v�̗����Ƀ`�F�b�N����ꂽ��ԂŃh���b�O����ƃG���[

//	TODO:	�����s�e���v�����̎�^�u�Ńz�C�[���X�N���[���������Ȃ��H�t�H�[�J�X�̖�肩�H
//			���C�����̕ҏW�X�y�[�X�APage List�A�����s�e���v������AA�ꗗ�c���[�ȂǁA���̏ꏊ�ł͖��Ȃ�

//	TODO:	Viewer�A�E�N�����j���[����̃h���t�g�{�[�h���J���Ȃ��H�E�ǉ�����Ȃ��H




//�@�\�ǉ��E�ύX
//	TODO:	�S�ł̈ꊇ�s���폜
//	TODO:	�����@�\���܂Ƃ߂Ď��s����}�N���̂悤�Ȃ��́��v���O�C���ł����邩�H

//	TODO:	�ł̕����I�����Ĉړ��A�c���c��
//	TODO:	ALT+N���A�Ŗ��̈ꊇ�ҏW�ɂ���H
//	TODO:	�ňꗗ���A�����I�����Ĉꔭ�폜�ł���悤��

//	TODO:	��������J�����Ƃ����t�@�C���E�v���t�����������烊�X�g��������ׂ�

//	TODO:	�t���[�J�[�\�����[�h
//	TODO:	���C���{�b�N�X���̂̑���A�㏑�����s�Ƃ����L�{�h�V���g�J�g�ŏo����悤��

//	TODO:	�E����������
//	TODO:	�����̌����E�s���@�\	���j�R�[�h�󔒈ꊇ�����Ƃ�
//	TODO:	�t�@�C�����J�������@�\
//	TODO:	�N�����ɁA���C���e���v���[�g�̃��X�g���J�����ēǂݍ��݂ł���悤�ɂ��ė~����//	Brush���������B�E�N�����j���[��

//	TODO:	�����t�@�C���^�u�A�E�N�����j���[�Ńt�@�C�����o��
//	TODO:	Ctrl+K�̓��v�@�\�EPageList�̍��ڑ��₵�đΏ��ECtrl+K�Ń��t���b�V���Ƃ��E�A�����p�Ƃ��A�s���󔒂Ƃ��E�����ȏ��������E�������^�C�����Əd���̂ő���Ń��t���b�V��
//	TODO:	�t�@�C�����e���v�@�\���~�����B�S�o�C�g���A�Ő��Ƃ�


//	TODO:	�g�p���t�@�C���o�͂ł���悤��
//	TODO:	DocBadSpaceCheck���̍X�V�w�߂̍œK�����K�v
//	TODO:	�o�b�N�A�b�v�@�\�̋����E�㏑���ۑ�������o�b�N�A�b�v�Ƃ��E�ŐV�K�쐬������ۑ�
//	TODO:	�ۑ�����Ƃ��A�����t�@�C������������A�����������ăo�b�N�A�b�v�Ƃ�
//	TODO:	�؂蔲�����t�@�����XAA���g���āA�����̂Ƃ������؂蔲���A�Ȃ��Ƃ��낾���؂蔲���A�Ƃ��ł���悤�ɂ�����
//	TODO:	�o�b�N�A�b�v���b�Z�[�W�A�X�e�[�^�X�o�[�ɏo���Ȃ���
//	TODO:	�S���ۑ����
//	TODO:	�e���v���̃R���{�b�N�X���A���X�g�{�b�N�X�^�Ɛ؂�ւ�����Ƃ�
//	TODO:	�v���r���[���������^�C���ɕύX����
//	TODO:	�񃆃j�R�̂Ƃ��́A�s�����h�̗ʂ̑I�����H�������͐ݒ�o����悤�ɂ��邩
//	TODO:	�~�j�����s�e���v���E��s�u���V�̃T�u�_�C�����O�ɑ��₷��

//	TODO:	View�������T���l�\���ł���悤��
//	TODO:	�e���v���́u��ɍőO�ʂɕ\���vOrinrinediter��\�����Ă��鎞�����őO�ʂɗ���悤��//�������́A���C�����ɓ����Ƃ�
//	TODO:	Viewer���ACtrl�����ŁAMLT�̃Z�N�V�����Ԉړ�����
//	TODO:	viewer�̎�^�u���}�E�X�ƃX�N���[���o�[�����łȂ��A�z�E�B�[���A�L�[�{�[�h�̖���PageUp,PageDn�L�[�ő���o����l�ɂȂ�Ȃ��H
//	TODO:	Viewer���A�|�b�v�A�b�v�ŁA�e�ʂ��o���Ȃ���
//	TODO:	MLT�̃u�b�N�}�[�N�@�\�E�^�u���₷���A�c���[�ɑ��₷���E���^�u���Ⴂ���Ȃ��H
//	TODO:	view�̎g�p�̃O���[�v���̕ύX�@�\�E�O���[�v���́A�P�̗�����
//	TODO:	View�̉E�N�����j���[���ҏW�o����悤��
//	TODO:	MMA�r���[�AAA�̃X�N���[�����A�C�e����؂�łȂ��s�X�N���[�����o����悤��
//	TODO:	MAA���������ł��Ȃ���

//	TODO:	ASD�Ή����Ǎ��͍�������ǂ��Ŏg���H
//	TODO:	�c�[���`�b�v�t������

//	TODO:	�g���X�̂P��ʁA�Q��ʐ؂�ւ����[�h
//	TODO:	�g���X�̂Q��ʂ��ƁA�w�i�G�͂ǂ����̃y�C���ł��\���o����悤�ɂ�����
//	TODO:	�g���X�ŁA�摜���܂�Œ��ڈړ��ł���悤��
//	TODO:	�g���X�@�\�A�_�ŃR���g���X�g�H�ƋP�x�����̕��������̂����傢�Ƌꂵ���炵��
//	TODO:	�g���X���[�h���ɁA�O���b�hON/OFF��t����
//	TODO:	�g���X���[�h�A�e���͒l�͒����͂ł���悤��
//	TODO:	�g���X���[�h�{�^���́ATOGGLE�ł���悤�ɂ���

//	TODO:	�v���t�@�C���̃c���[�̕ҏW�A�����Ǝ�y�ɂł��Ȃ���




//OK?
//	�V�K�ŊJ���āA����𖼂Â��ۑ������t�@�C���́A�����ɓ������ĂȂ�
//	�ō폜��Alt+D�t����B���Ӄ_�C�����O�o���B
//	�T���l���炾�Ǝg�p�����ɔ��f����Ȃ�
//	�����`�`�̓��͂Ƃ��A���C���{�b�N�X�̕ҏW�Ƃ��ŁA�L�[�{�[�h�V���[�g�J�b�g�g����悤��
//	�g���X�����A�ő剻���Ă�ƉB��Ă��܂�
//	�ő}���_�C�����O�̃`�F�b�N�{�b�N�X�o���Ă����悤��


//	TODO:	�n�j�H	�l�`�`�e���v���̃X�e�[�^�X�o�[�y�[�W�ԍ��̕\�����O��Ȃ̂łP�ɂ���

//	TODO:	�n�j�H	�c�[���o�[�ɂ�Accelerator�L�[�\������
//	TODO:	�n�j�H	�o�C�g���x����4096�͒����o����悤��
//	TODO:	�n�j�H	�E���[���[�݂����ȁA�s���[���[�i�R�O�s�Ƃ��j��	IDM_UNDER_RULER_TOGGLE
//	TODO:	�n�j�H	���j�R�[�h���X�g�E�g�������c�̋L�^�Ƃ�

//	TODO:	�n�j�H	�f�B���C���[�h�E�����[�h�ł̑S�v���r���[����������
//	TODO:	�n�j�H	�g���Ƃ��̓s�����h�����g��
//	TODO:	�n�j�H	�ő剻����߂�����AMAA����\���ł��\���ɂȂ�
//	TODO:	�n�j�H	MAA�ƕňꗗ�A�I�������烁�C�����Ƀt�H�[�J�X�߂����ǂ����I���o����悤�ɂ���
//					�E�N�����j���[��ON/OFF�t����΂���
//	TODO:	�n�j�H	�h���b�O�ړ���������A�L�����b�g�̈ʒu����������
//	TODO:	�n�j�H	�h���t�g�{�[�h�E�T���l�C���E�ړ��o����悤��
//	TODO:	�n�j�H	���[�U�[��`���A�N�Z���L�[�Őݒ�o����
//	TODO:	�n�j�H	�Z���t���s�A���͂̓r���Ŕ��p�S�p�X�y�[�X��������ꍇ�A�\�R�͖������ĕ��͂̐�[�ɍs���𑵂���
//	TODO:	�n�j�H	�c�[���`�b�v�A�}�E�X�J�[�\�����d�Ȃ�Ƃ�����͎̂d�l�B���삵���Ȃ����B
//	TODO:	�n�j�H	�E�N�����j���[�̌ʘg�⃆�[�U�A�C�e���Ɋe�̖��O��t����悤��
//	TODO:	�n�j�H	���^�u�̖��̕ҏW�o����悤��
//	TODO:	�n�j�H	�󔒂̕\����\��������������
//	TODO:	�n�j�H	��������Ƃ��A��G�̎��͂𔒃k�L����@�\
//	TODO:	�n�j�H	���C���{�b�N�X���k�L�A�P�U�E�Q�Q�Ƃ��A�������p���[������Ă���
//	TODO:	�n�j�H	MLT�����A������Ȃ��Ȃ猟�����Ȃ��悤�ɂ���
//	TODO:	�n�j�H	viewer���̕\���F�ύX�E�w�i�Ƃ�
//	TODO:	�n�j�H	�I��͈͂�AST�̕Ŗ��ɂ���@�\	IDM_PAGENAME_SELASSIGN
//	TODO:	�n�j�H	�v���r���[�ĕ`�悵�Ă��AALL�ƕ\���ʒu���ێ�����


/*

ASD�t�@�C���@�@��s����R���e���c

���e�̉��s�@�@�@�@�@0x01,0x01
���e�Ɛ����̋�؂�@0x02,0x02

*/
//	v ^ (v >> 1)	�O���C�R�[�h

/*
2011/06/30	0.10	�������[�X�A�v���A���t�@��
2011/07/04	0.11	�I�𒆂ɉE�N������Ɖ������ꂿ�Ⴄ�̂��C��
					�S�I�����ăR�s�[�����Ƃ��̖��[���s���B���C��
					���C���{�b�N�X�ŁA�ҏW�r���[�̃X�N���[�����l������ĂȂ������̂��C��
					MAA�ŕ����t�@�C�����^�u�ŊJ������悤�ɂ���
2011/07/19	0.12	�g�̕ێ����P�O�ɂ���
					�I�����̕��^�u�𕜋A�o����悤�ɂ���
					�V�K�ۑ��⃊�l�[���ۑ����ƁA���X�g�I�[�|�����L�^����Ȃ��̂��C��
					PageList�ŁA�폜�E�ړ��ł���悤�ɂ���
2011/07/20	0.13	MLT�c���[�ŁAAST���ǂ߂�悤�ɂ���
					�u���V�A��s�e���v���̃R���{�b�N�X�̓}�E�X�z�C�[�����Ȃ��悤�ɂ���
					�I����ԉ�������Ă邯�ǃt���O����������ĂȂ��̂��C����������
					��`�I��͈͂��A�J�[�\���ʒu�ɏ�����悤�ɂȂ����H
					�؂���̃L�[�{�[�h�A�N�Z�����[�^�����������E�P�Ȃ����Y��
					�O�����̍s��Alt�{�������Ƃ��ځ[�񂵂Ă��̂��C��
					MAA�e���v���ŁA�c�[���`�b�v��ON/OFF�o����悤�ɂ���
					�g�ɖ��O�t������悤�ɂ���
					�g���X�@�\�����E�������P���K�v
					�I��͈͂𔽓]�\�������Ⴄ
					�e���v���t�@�C�����ATemplates�f�B���N�g���Ɉړ�
2011/08/01	0.14	�R���e�L�X�g���j���[�ɂ��A�N�Z�����[�^�L�[��t����
					�g���X�F�\����\���� Alt+I �ŏo����悤�ɂ�����
					�g���X�F��\�����͕����F�߂��悤�ɂ���
					�g���X�F����̑��삵����t�H�[�J�X���`��r���[�ɖ߂�悤�ɂ���
					�u���V�ƈ�s�e���v���̕\�����i�q��ɂ����E�ł��I���̐F�ς����w��
					MAA�̃c�[���`�b�v�����T�C�Y���X�ƂP�Q�őI���o����悤�ɂ���
					���C���{�b�N�X�A�\��t�����炷�����邩�ǂ����̑I���`�F�b�N����
					���j�R�[�h�p�f�B���O���g����悤�ɂ���
2011/08/09	0.15	�g���X���̓^�C�g���o�[��[�g���X���[�h]����
					�g���X�̕\����\����Alt+H�ɕύX
					�g���X�ŁA�G���J������Y�[�����P�O�O���ɖ߂��悤�ɂ���
					�g���X�̉摜�J�����ɃT���l�C���t����
					�ňꗗ�Ƀc�[���o�[�t����
					�ł̕����@�\������
					Alt���E�̏ꍇ�ɂ����j�R�[�h�����ăp�f�B���O����悤�ɂ���
					Ctrl+Home,End�������悤�ɂ���
					MAA���Ƀt�H�[�J�X������Ƃ��ATAB,Shift+TAB�Ń^�u��؂�ւ��悤�ɂ���
					MAA�̕��^�u���폜�E�ǉ�������I�������������Ȃ��Ă�̂��C��
					���j�R�[�h���l�Q�ƁA�P�U�i���^�ɑΉ�
					���C���{�b�N�X�A�u�b���ޓ��e�ɍ��킹�āA���̑傫�����g�傷��悤�ɂ���
					�����o�b�N�A�b�v�@�\��t����
2011/08/18	0.16	�L���v�V�����o�[�̊J���Ă�t�@�C�������ύX����Ȃ���������̂��C��
					���̎Q�Ƃ̓Ǎ�������悤�ɂ����B�����������Ń��j�R�[�h�ɂ��Ă�
					�t�@�C���J�����Ƃ��ɁA�y�[�W���X�g�Ƀo�C�g���Ƃ������f����Ȃ��̂��C��
					���C�������A����邱�Ƃ��������̂��C��
					���C���{�b�N�X�̓��ߍ������o����悤�ɂ���
					�N���b�v�{�[�h���e�L�X�g�̃��j�R�[�h���ʏo����悤�ɂ���
					�c�[���`�b�v���AWin7���ƃ`�����̂����Ȃ��悤�ɂł����H
					�y�[�W���X�g�ɁA�e�ł̃T���l�C�����ۂ��̂��|�b�p�b�v����悤�ɂ���
					MAA�E�C���h�E��AA�\�����A���j�R�[�h�ɕϊ����ĕ\������悤�ɂ����E���Ȃ��Ǝv������
					���C���{�b�N�X�̃T�C�Y�ύX�����������̂𒼂���
					AAViewer�����𕪗����ĒP��Project��������悤�ɂ���
2011/08/24	0.17	�o�b�N�A�b�v�f�B���N�g������ɍ쐬����悤�ɂ���
					�o�b�N�A�b�v�������b�Z�[�W��ON/OFF�o����悤�ɂ����E�~�X�͏�ɕ\��
					�ł�V�K�쐬�����Ƃ����o�b�N�A�b�v����悤�ɂ���
					�o�C�g���v�Z�̍�蒼���E�������m�ɏo����悤�ɂȂ���
					ALT�㉺�ŕŐ؊��ł���
					�h���b�O�I�������₷������
					�ł̓����ƕ������o����悤�ɂ���
					���^�u�́A�Ō�Ɍ��Ă����ʒu���o���Ă�����悤�ɂ����E�I�������������
2011/08/31	0.18	�����t�@�C���I�[�|���o����悤�ɂȂ����͂��E���v������
					Ctrl+F4 �Ń^�u���ł���
					ALT�{�����ŕňړ�������APageList���X�N���[������悤�ɂ���
					�c�[���o�[�Ɂu��蒼���v��ǉ�
					�󔒂̕\���E��\����؂�ւ�����悤�ɂ���
					�I��͈͂��A�󔒂��u���V�œh��Ԃ���悤�ɂ���
					�e�Ɨ������A�őO�ʕ\���o����悤�ɂ���
					�������o�ł���悤�ɂȂ�������
					�J�����g�f�B���N�g�����AEXE�t�@�C���̂���ꏊ�Ŏ��悤�ɂ���
					�o�O�C�����낢��
2011/09/06	0.19	�R�}���h���C������̃t�@�C���I�[�|���ɑΉ������H�ŏ��̈����
					�󔒌x������������A�s�ԍ����g���Ȃ�悤�ɂ���
					�ŏI�ō폜�����琁����Ԃ̂��C��
					�O���b�h����\���o����悤�ɂ���
					�E�[���[����\���ł���悤�ɂ���
					�w�i�F���ύX�ł���悤�ɂ����B�ł�GUI���Ȃ�
					���̕ł̑O�ɑ}���A���A��ɑ}���ɂ����E�����쐬�̃L�[�o�C���h�ύX
					�E�N�����j���[�F�X�ǉ�
					�}�E�X�z�C�[���̃X�N���[���ʂ��n�r�W�����g���悤�ɂ����E����������
					PageUp,PageDn�łP�O�s���X�N���[������悤�ɂ���
					�s��������Əd���Ȃ�̂��Ȃ��悤�ɂ����E�����C�P�Ă�
					�s���������U���܂őΉ�
					�����t�@�C���Ԃ̈ړ����ACtrl+PageUp,Down�ňړ��ł���悤�ɂ���
					IE�R���|�[�l���g���g�����ȈՃv���r���[����
					Ctrl+Home,End�ŁA�X�N���[������肭�����Ȃ��̂��C��
					�ŏ��Ԉړ��̏㉺���t�������̂��C��
					�Ń��X�g�A�w�b�_�̕��ύX���Ă����X�g�̂ق����ς��Ȃ��̂��C���E���v���ȁH
					�o�O�C�����낢��
2011/09/16	0.20	���C���{�b�N�X�̓��e���N���b�v�{�[�h�ł���悤�ɂ���
					�����s�e���v������ҏW�r���[�ɊJ������悤�ɂ���
					�v���r���[�@�\���A<DD>�Ƃ��g���Ď��ۂ̉�ʂ��ۂ��Ȃ�悤�ɂ���
					��ʂ�ALL�v���r���[���Ă����Ȃ��Ȃ���������
					�����x���@�\�Ƃ��āA�ő���SJIS�R�s�[�@�\��ǉ��B�z�b�g�L�[[Ctrl+Shift+D]���g�p�B
					���s�}�[�N[CrLfMark]�A��SJIS����[CantSjis]�̐F�ύX�o����悤�ɂ���
					�c�[���o�[�����o�[�ɂ����B�ł��ύX�̕������ł��Ă˂�
					�E�N�����j���[�̘g�̖��̂��\�������悤�ɂ���
					���o����Alt+Space����������A���o���C���{�b�N�X�J���ł�����
					����ɔ����A���o���ă��C���{�b�N�X�֋@�\�͎����
					���o����UNICODE�R�s�[���o����悤�ɂȂ�������
					��`�E�h�ׁE���o�E�󔒂̏�Ԃ��X�e�[�^�X�o�[�ɕ\��
					���[�󔒍폜���Ă��A�s���\�����Ԃ��܂܂������̂��C��
					�����Ă�ł��ABMP��PNG�ŕۑ��o����悤�ɂ���
2011/09/30	0.21	�X�y�[�X���\���ɂ��Ă��A�x���͕\������悤�ɂ���
					�g���X���[�h�̊G�̏�ɁA�O���b�h���o���悤�ɂ���
					�c�[���o�[�A���j���[�́A�g���X���[�h�E���j�R�[�h�\��TOGGLE����ɂ���
					���j�R�[�h�\�Ń_�u���N���b�N������}�������悤�ɂ���
					�ŕ����E�����E�폜�̊m�F���b�Z�[�W�ɁA������\�����Ȃ��I�v�V����
					�J�[�\���ʒu���o����悤�ɂ����E���܂����Ȃ���
					�S�̂̃h�b�g�V�t�g�@�\�E�I��͈͂̂ݏ������ł���
					�����s�e���v���̋�؂���A���ƐF�Ⴂ��؂�ւ�����悤�ɂ���
					�����s�e���v���ŁA�v���t�@�C���@�\�Ő؂�ւ�����悤�ɂ���
					�����s�e���v���v���t�@�C���ɁA�c���[���e���L���b�V������
					�����s�e���v���̎g�p���X�g�̕��ނ��ڂ�������
					�����s�e���v���ŁA�g�p���X�g���^�u�ŊJ����悤�ɂ���
					Viewer���o�[�W�����A�b�v
2011/10/20	0.22	�����̊g���q���AAST�Ȃ炻����ێ�����悤�ɂ���
					�g���X���[�h�A�X���C�_���X�N���[���o�[�ɂ��Ă݂�
					�ҏW�G���A�̃X�N���[���ɍ��킹�ĊG�������悤�ɂ���
					�������������x��������ꂽ������
					�u���V�E��s�e���v���ŁA�e�A�C�e���̉������c�[���`�b�v�ɕ\���E�ł��Ȃ񂩃w��
					�I�����ē��́��폜�A��`�I�����h��ׂ��A����悤�ɂł���������
					�g�}�������쐬
					���C���{�b�N�X�A�g�}�������A�J�[�\���L�[�ňٓ��o����悤�ɂ���
					���e�ύX������A�t�@�C���^�u��[�ύX]
					�őS�̂��󔒂Ŗ��߂�@�\
					�Ŕԍ��}���@�\
					��s�e���v���A�u���V�e���v���A�Ń��X�g�����C�����ɂ�������@�\
					�����s�e���v���̃^�u���A�����i�\���o����悤�ɂ���
					�o�O�C��������
2011/11/07	0.23	�I��͈͂̃o�C�g���\������悤�ɂ���
					�g�}�����ɁA�}�����������X�C�b�`�𓋍�
					�����s�e���v������̊����}�����A�����Ƌ�`�}���Ȃ�悤�ɂ���
					���C���{�b�N�X�J���Ƃ��A�J�[�\���ʒu�ɊJ���悤�ɂ�����
					�����s�e���v���Ɍ��o���@�\��t�����BAST�ƍs���́y�z�ɔ���
					�����s�e���v���̃c���[�̃t�@�C���ւ́A���e�ǉ��ł���悤�ɂ�����
					�u���V����Ƃ��ɁA�h�b�g���͂ݏo���Ă��C�ɂ��Ȃ��悤�ɂ���
					�R�s�[���[�h���AUnicode��ShiftJIS�����ւ�����悤�ɂ���
					�����`�`�ϊ��@�\�o��������
					�o�O�C��������
2011/11/11	0.24	�R���e�L�X�g���j���[�ҏW�@�\�𓋍ڂł������H
					Ctrl�{�����ŁA�e���v���O���[�v�̕ő���
					�����s�e���v���̌��o���@�\�������B�}�V�ɂȂ������H
					�o�O�C�����낢��
2011/11/14	0.24.1	�����`�`�ϊ������A��ʊO�ɍs���Ȃ��悤�ɂ���
					���d�N���h�~���ꂽ
					�v���r���[�E�C���h�E�̈ʒu�Ƒ傫�����o���Ă����悤�ɂ���
					�g�}�������Ƃ��ɉ��̕����X�V����Ȃ��̂��C��
					�o�O�C�����낢��
2011/11/28	0.25	MAA�̃t�@�C���������@�\
					MAA�̃c���[�W�J�������Ȃ����C������iViewer���݁j
					�v���t�@�C���\�z�ŁA�����̃v���t�ƈ�v����Ȃ�`�F�L����悤�ɂ����iViewer���݁j
					�v���t�@�C�����Ƃ��̎��Ԃ��Z�k�ł����C������iViewer���݁j
					���C�����̃e���v���G���A�̃T�C�Y�ςɂȂ���
					�ő剻��Ԃ��o���Ă����悤�ɂ���
					4096�o�C�g��������A�Ń��X�g�̃o�C�g���̂Ƃ���Ԃ�����悤�ɂ���
2011/12/22	0.26	�h���t�g�{�[�h�@�\�iViewer���݁j
					MAA�T���l�C���@�\�iViewer���݁j
					MAA�̎g�p�̃O���[�v�ꊇ�폜�iViewer���݁j
					�z�F�ύX���f�t�h�ŏo����悤�ɂ���
					�I�[�|���L�^���Ȃ���΁A����AST��\������悤�ɂ���
					�t�@�C������Caret�ʒu��ۑ�����悤�ɂ���
					�N�������܂܁A��s�e���v���A�u���V�e���v���̃����[�h�ł���悤�񂵂�
					�N������Ƃ��A�Ō�ɊJ���Ă��t�@�C�����J���Ƃ��I���ł���悤�ɂ���
2012/01/16	0.27	�ҏW���̃t�@�C���^�u�ŁA�}�E�X�I�[�o�[�Ńt���p�X�\��
					�l�`�`�ƃh���t�g�{�[�h�ŁA���N���b�N�̏ꍇ�����𑝂₵���iViewer���݁j
					�䎌�pEnter�@�\�BShift+Enter�ŁA���s�}���ł͂Ȃ����̍s�ɋ󔒂Ƃ��ő䎌�ʒu���킹�ł���
					�c�����@�\
					�o�O�C�����낢��
2012/02/29	0.28	�t�s�e�W�ŃG�N�X�|�[�g�o����悤�ɂ���
					�t�H���g�ύX�@�\
					�ō폜��Alt+D�ǉ�
					�T���l����g������A�g�p�^�u�ɒǉ�����Ȃ��̂��C��
					���o�[�̈ʒu�L���Ə�����Ԃɖ߂�����
					�t�@�C���J����������ێ��ł���悤�ɂ���
					MAA�����Ɂ���PageUpDown�Ƃ��ǉ��ł�����������Ȃ�
					MAA�̃��X�g�A�b�v�Ώۂ�TXT��ǉ�
					���s�������ƃp�t�H�[�}���X��������̂��������P�ł����C������
					�I�����̊m�F���b�Z�[�W�ύX
					IE�v���r���[��CSS�������ύX�E�s�Ԃɂ���
					�o�O�C�����낢��
2012/03/24	0.29	�䎌���s�A�󔒍s�Ŏ��s�����ꍇ�́A�L�����b�g�ʒu�Ŏ��̍s���m�ۂ���悤�ɂ���
					Editor�Ō����ĂȂ������v���t�@�C���������C��
					�E�������A���j�R�[�h�ɑΉ�
					�s�����p�󔒂����j�R�[�h�󔒂ɒu��������@�\����
					Ctrl+B�Ńv���r���[�J���悤�ɂ���
					��s�A�u���V�e���v���̕\��/��\�����ł���
					�A�N�Z���L�[�ҏW�@�\�𓋍�
					�A���h�D���h�D������Ƃ��͑I��͈͉�������悤�ɂ���
					�߂ɊJ���Ă���t�@�C�����J�����Ƃ�����A���̃^�u�Ɉڂ�悤�ɂ���
2012/05/15	0.30	MAA����ENTER��������A���̂Ƃ��g�b�v�Ɍ����Ă�AA�����N���b�N���삷��悤�ɂ���
					������Ⴕ���͋󔒗�Ń_�u���N���b�N����ƁA���͈̔͂�I����Ԃɂ���
					�I��͈͂��h���b�O�ړ��o����悤�ɂ����E������Ƌ��������������H
					�v���r���[�J���Ă���A�ۑ������Ƃ��ɍĕ`�悷��悤�ɂ���
					�g�쐬�ɁA�����s�g����悤�ɂ���
					�g�̌����Q�O�ێ��ł���悤�ɂ���
					�c�[���o�[�ɁA���j�R�[�h�󔒂̎g�p�^�s�g�p�g�O���{�^������
					���E���]�A�㉺���]�@�\��ǉ�
					���C���{�b�N�X�ɁA���e�폜�{�^����ǉ�
					4096byte��������A�X�e�[�^�X�o�[�̃o�C�g�\���ʒu���Ԃ��Ȃ�B
					�ő}���_�C�����O�̃`�F�b�N�{�b�N�X�o���Ă����悤�ɂ���
					�L�����b�g�̈ʒu�ɋ�`�\�t�@�\��ǉ�
					��s�e���v���Œ��N���b�N�����烌�C���{�b�N�X���J��
					MAA�c���[�Ńt�@�C�����𒆃N���b�N������A���^�u�ɒǉ��ł���
					�o�O�C�����낢��
2012/09/10	0.31	MAA�ɁA�t�@�C���̓r���ɃA�C�e����ǉ��ł���悤�ɂ���
					MAA�̓��e�\�����ɂc���c����ƁA���̃t�@�C���𕛃^�u�ŊJ���iViewer���݁j
					���g�p�ɂ͓���Ȃ�������������Ȃ�
					MAA�c���[�ɂc���c����ƁA�ǉ��A�C�e���Ƃ��ăc���[�ɒǉ��iViewer���݁j
					���č\�z����Ə�����
					MAA�̎g�p�ŁA������MLT�ɓ���AA���L��ꍇ�ɃL���h��̂��C���B�����iViewer���݁j
					�g�}��BOX���k�߂�����ƃG���[�ɂȂ�̂��C��
					�䎌���s�ŁA�P�ƃX�y�[�X�̓m�[�J���ɂ���
					���[�U�A�C�e�����E�N�����j���[�ɒǉ��ł���悤�ɂ���
					�h���b�O�ړ���������L�����b�g�ʒu�����������Ȃ�̂��C��
					MAA���ƕňꗗ�ŁA�I��������t�H�[�J�X��ҏW���ɖ߂��@�\
					�h���t�g�{�[�h�E�T���l�C���̑����ړ��o����悤�ɂ���
					�c�[���`�b�v���`�����̂��Ȃ�Ƃ��o�����C������
					���^�u�̖��̕ύX���o����悤�ɂ����iViewer���݁j
					�t�@�C���I�[�|�����������o�����C������
					�󔒂̕\��/��\����Ԃ��o���Ă����悤�ɂ����B
					���C���{�b�N�X�\�t�ɔ��k�L�@�\�ǉ�
					���N���b�N�ŁAMAA�̕��^�u��ҏW�t�@�C��������悤�ɂ����iViewer���݁j
					�s�����[���[��t����
					�o�C�g���I�[�o�[�x����ݒ�ł���悤�ɂ���
					���j�R�[�h�\�ɁA�g�p����������
					�l�`�`���̔w�i�F�ύX�o����悤�ɂ���
					�v���r���[���ĕ`�悵�Ă��A���̂Ƃ��J���Ă����ʒu�Ə�Ԃ��ێ��ł���A�͂�
					�I��͈͂�AST�Ŗ��̂ɂ���@�\�ǉ�
2013/05/31	0.32	MAA�̃c���[����AA�ł��폜�o����悤�ɂ���
					�V�K�ۑ����̊g���q���AMLT��AST��K�p����悤�ɂ���
					�h���t�g�{�[�h�E�T���l�C���̃T�C�Y���S�i�K�ŕς����悤�ɂ���
					MAA���ɁA�J���Ă�œ��̕����񌟍���t�����E
					AST�ɒǉ�����ƃZ�p���[�^�����������Ȃ�̂��C��
					�Ő؂�ւ������A�Ō�̕ł̂Ƃ���܂ŕňꗗ���X�N���[��
					�C�ӂ̏ꏊ����E�̕������A���E�ɂ��炷�@�\
					MAA�e���v���ɃA�C�e���ǉ�������S�A�C�e���ɉ��s��������̂��C��
					��ڂ��N�������Ƃ��A�����ɓn����Ă���A�����̂�ł�����J���悤�ɂ���
					���d�N����I���ł���悤�ɂ������E���؂��K�v
					�ۑ������Ƃ��̃o���[�����b�Z�[�W�̕\����ON/OFF�o����悤�ɂ���
					MAA���̕��^�u���A���i�\���ƈ�s�\����؂�ւ�����悤�ɂ���
					���C�����T�C�Y�ς����Ƃ��ɁA�X�v���b�g�o�[���ǐ�����悤�ɂȂ�������
					�����[�h�̕ł𓝍�������A���e��������̂��C��
2014/04/01	0.33	�����̃q���g�@�\��ǉ�
					�{���̒P�ꌟ���@�\�������B�����C�P�Ă�B
					������Ύd�l�ύX�ɂ�镶����������@��ˑ��������A���j�R�[�h���l�Q�ƂƂ��ăR�s�[�o����悤�ɂ��Ă݂��i�ꓖ����I�Ή��j
2015/01/23	0.34	�g�ҏW�ŁA���O�ɂȂ�悤�Ȏw�肪�����ƃA�v�����u�b��Ԃ̂��C���B�������g�\���͂��������Ȃ�B
					AST�ł�MLT�ł��Ȃ��A�P�O�O�s�ȏ゠��t�@�C���i�����x�^�e�L�X�g�j�ɂ��āA�������Ď�荞�ދ@�\��ǉ�
					�@�i���������j�S�O�s�ȍ~�łS�s�ȏ�̋󂫂����邩�A�P�O�O�s�ȍ~�łP�s�ȏ�̋󂫂����邩�A�Q�T�U�s�ȏ㑱���Ă���
					MLT2HTML���g����HTML�G�N�X�|�[�g�@�\��ǉ�
					������Ύd�l�ύX�ɂ�镶�����������̊g��ɑΉ��B����������C������B
2015/01/26	0341	�����Ǎ�������Ԃ��Ƃԏꍇ������̂��C��

�@��ˑ������̃o�C�g�J�E���g�𔽉f����̂́A�A�v���ċN�����K�v


�X�V��������

*/

/*
IME���̃Q�b�g

	HKL	hKl;
	TCHAR	atImeName[MAX_STRING];
	hKl = GetKeyboardLayout( 0 );
	ImmGetDescription( hKl, atImeName, MAX_STRING );
*/

//-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "OrinrinEditor.h"
//-------------------------------------------------------------------------------------------------

static CONST TCHAR	gcatLicense[] = {
TEXT("���̃v���O�����̓t���[�\�t�g�E�F�A�ł��B���Ȃ��͂�����A�t���[�\�t�g�E�F�A���c�ɂ���Ĕ��s���ꂽGNU��ʌ��O���p������(�o�[�W����3���A����ȍ~�̃o�[�W�����̂����ǂꂩ)����߂�����̉��ōĔЕz�܂��͉��ς��邱�Ƃ��ł��܂��B\r\n\r\n")
TEXT("���̃v���O�����͗L�p�ł��邱�Ƃ�����ĔЕz����܂����A*�S���̖��ۏ�*�ł��B���Ɖ\���̕ۏ؂����ړI�ւ̓K�����́A���O�Ɏ����ꂽ���̂��܂߁A�S�����݂��܂���B\r\n\r\n")
TEXT("�ڂ�����GNU��ʌ��O���p���������������������B\r\n\r\n")
TEXT("���Ȃ��͂��̃v���O�����Ƌ��ɁAGNU��ʌ��O���p�������̃R�s�[���ꕔ�󂯎���Ă���͂��ł��B\r\n\r\n")
TEXT("�����󂯎���Ă��Ȃ���΁A<http://www.gnu.org/licenses/> ���������������B\r\n\r\n")
};
//-------------------------------------------------------------------------------------------------

//	�\���p�t�H���g�x�[�X�e�[�u���E������R�s�[���Ďg��
static LOGFONT	gstBaseFont = {
	FONTSZ_NORMAL,			//	�t�H���g�̍���
	0,						//	���ϕ�
	0,						//	��������̕�����X���Ƃ̊p�x
	0,						//	�x�[�X���C����X���Ƃ̊p�x
	FW_NORMAL,				//	�����̑���(0~1000�܂ŁE400=nomal)
	FALSE,					//	�C�^���b�N��
	FALSE,					//	�A���_�[���C��
	FALSE,					//	�ł�������
	DEFAULT_CHARSET,		//	�����Z�b�g
	OUT_OUTLINE_PRECIS,		//	�o�͐��x
	CLIP_DEFAULT_PRECIS,	//	�N���b�s���O���x
	PROOF_QUALITY,			//	�o�͕i��
	VARIABLE_PITCH,			//	�Œ蕝���ϕ�
	TEXT("�l�r �o�S�V�b�N")	//	�t�H���g��
};
//-------------------------------------------------------------------------------------------------

// �O���[�o���ϐ�:
static  HINSTANCE	ghInst;						//!<	���݂̃C���^�[�t�F�C�X
static  TCHAR		gszTitle[MAX_STRING];		//!<	�^�C�g���o�[�̃e�L�X�g
static  TCHAR		gszWindowClass[MAX_STRING];	//!<	���C���E�B���h�E�N���X��

static  HMENU		ghMenu;			//!<	���j���[

static	HACCEL		ghAccelTable;	//!<	�L�[�{�[�h�A�N�Z�����[�^�n���h��

static  HWND		ghFileTabWnd;	//!<	�����t�@�C���^�u
static  HWND		ghFileTabTip;	//!<	�����t�@�C���^�u�c�[���`�b�v
static WNDPROC		gpfOriginFileTabProc;	//!<	�����t�@�C���^�u�̌��v���V�[�W��

static  HWND		ghMainWnd;		//!<	���C���E�C���h�E�n���h��
static  HWND		ghStsBarWnd;	//!<	�X�e�[�^�X�o�[
static  HBRUSH		ghStsRedBrush;	//!<	�X�e�[�^�X�o�[�p�g�u���V

static HANDLE		ghMutex;		//!<	���d�N���h�~�pMutex

static INT			giResizeWide;	//!<	�E�C���h�E�T�C�Y�ύX�����Ƃ��A�ύX�O�Ƃ̍����E�g���ĂȂ�

#ifdef WORK_LOG_OUT
static HANDLE		ghLogFlie;		//!<	���O�o��
#endif

EXTERNED HFONT		ghNameFont;		//!<	�t�@�C���^�u�p�t�H���g

EXTERNED HWND		ghMaaWnd;		//!<	�����s�`�`�e���v��
EXTERNED HWND		ghPgVwWnd;		//!<	�y�[�W���X�g
EXTERNED HWND		ghLnTmplWnd;	//!<	��s�e���v��
EXTERNED HWND		ghBrTmplWnd;	//!<	�u���V�e���v��
EXTERNED BOOLEAN	gbDockTmplView;	//!<	�������Ă�e���v���͌����Ă��邩

EXTERNED HWND		ghMainSplitWnd;	//!<	���C���̃X�v���b�g�o�[�n���h��
EXTERNED LONG		grdSplitPos;	//!<	�X�v���b�g�o�[�́A�����́A��ʉE����̃I�t�Z�b�g

EXTERNED UINT		gbUniPad;		//!<	�p�f�B���O�Ƀ��j�R�[�h�������āA�h�b�g�������Ȃ��悤�ɂ���
EXTERNED UINT		gbUniRadixHex;	//!<	���j�R�[�h���l�Q�Ƃ��P�U�i���ł��邩

static   UINT		gdBUInterval;	//!<	�o�b�N�A�b�v���o�E�f�t�H�R�����炢�H
EXTERNED UINT		gbAutoBUmsg;	//!<	�����o�b�N�A�b�v���b�Z�[�W�o�����H
EXTERNED UINT		gbCrLfCode;		//!<	���s�R�[�h�F�O������΁E��O�x�x 

EXTERNED UINT		gbSaveMsgOn;	//!<	�ۑ����b�Z�[�W�o�����H

EXTERNED UINT		gdPageByteMax;	//!<	�냌�X�̍ő�o�C�g��

static TCHAR		gatExePath[MAX_PATH];	//!<	���s�t�@�C���̈ʒu
static TCHAR		gatIniPath[MAX_PATH];	//!<	�h�m�h�t�@�C���̈ʒu

EXTERNED INT		gbTmpltDock;	//!<	�e���v���̃h�b�L���O

static list<OPENHIST>	gltOpenHist;	//!<	�t�@�C���J���������E
EXTERNED HMENU		ghHistyMenu;		//!<	����\�����镔���E���I�ɓ��e�쐬���Ȃ�����


static BOOLEAN		gbDummy;		//

#ifdef BIG_TEXT_SEPARATE
static BOOLEAN		gbSeparateMod;	//!<	�����Ǎ������^�u�ɕύX�}�[�N�K�v
#endif

#ifdef FIND_STRINGS
extern  HWND		ghFindDlg;		//	�����_�C�����O�̃n���h��
#endif
extern  HWND		ghMoziWnd;		//	�����`�`�ϊ��_�C�����O�̃n���h��

extern  HWND		ghMaaFindDlg;	//	MAA�����_�C�����O�n���h��

extern  UINT		 gdClickDrt;	//	�h���t�g�{�[�h�N���b�N�X�^�C��
extern  UINT		gdSubClickDrt;	//

extern  HWND		ghViewWnd;		//	�r���[

extern  UINT		 gdGridXpos;	//	�O���b�h���̂w�Ԋu
extern  UINT		 gdGridYpos;	//	�O���b�h���̂x�Ԋu
extern  UINT		gdRightRuler;	//	�E���̈ʒu�h�b�g
extern  UINT		gdUnderRuler;	//	�����̈ʒu�s��

#ifdef SPMOZI_ENCODE
extern  UINT		gbSpMoziEnc;	//!<	�@��ˑ������𐔒l�Q�ƃR�s�[����
#endif
//-------------------------------------------------------------------------------------------------

#ifdef PLUGIN_ENABLE

//[_16in] Add - 2012/05/01
//-- �v���O�C���֌W
plugin::PLUGIN_FILE_LIST	gPluginList;
#include "Plugin/PluginCtrlAsync.h"

#endif


//	�X�e�[�^�X�o�[�̋�؂�
#define SB_ITEMS	8
CONST INT	gadStsBarSize[] = { 50, 200, 350, 500, 700, 800, 900, -1 };
//-------------------------------------------------------------------------------------------------

VOID	Cls_OnActivate( HWND, UINT, HWND, BOOL );		//!<	
BOOLEAN	Cls_OnCreate( HWND, LPCREATESTRUCT );			//!<	�{�̂� WM_CREATE �̏����E�Œ�Edit�Ƃ�����
VOID	Cls_OnCommand( HWND , INT, HWND, UINT );		//!<	�{�̂� WM_COMMAND �̏���
VOID	Cls_OnPaint( HWND );							//!<	�{�̂� WM_PAINT �̏����E�g���`��Ƃ�
VOID	Cls_OnSize( HWND , UINT, INT, INT );			//!<	
VOID	Cls_OnMove( HWND, INT, INT );					//!<	
VOID	Cls_OnDestroy( HWND );							//!<	�{�̂� WM_DESTROY �̏����EBRUSH�Ƃ��̃I�u�W�F�N�g�̔j���Y��Ȃ��悤��
LRESULT	Cls_OnNotify( HWND , INT, LPNMHDR );			//!<	
VOID	Cls_OnTimer( HWND, UINT );						//!<	
VOID	Cls_OnDropFiles( HWND , HDROP );				//!<	
VOID	Cls_OnContextMenu(HWND,HWND,UINT,UINT );		//!<	
VOID	Cls_OnHotKey(HWND, INT, UINT, UINT );			//!<	
VOID	Cls_OnDrawItem( HWND, CONST DRAWITEMSTRUCT * );	//!<	

BOOL	Cls_OnWindowPosChanging( HWND, LPWINDOWPOS );	//!<	

#ifdef MULTIACT_RELAY
void	Cls_OnCopyData( HWND, HWND, PCOPYDATASTRUCT );	//!<	
#endif

INT_PTR	CALLBACK OptionDlgProc( HWND, UINT, WPARAM, LPARAM );	//!<	

LRESULT	CALLBACK gpfFileTabProc( HWND, UINT, WPARAM, LPARAM );	//!<	�����t�@�C���^�u�̃T�u�N���X�v���V�[�W��
VOID	Ftb_OnMButtonUp( HWND, INT, INT, UINT );	//!<	

HRESULT	ViewingFontNameLoad( VOID );	//!<	

#ifdef USE_NOTIFYICON
VOID	TaskTrayIconEvent( HWND, UINT, UINT );	//!<	
#endif
//-------------------------------------------------------------------------------------------------


/*!
	�A�v���P�[�V�����̃G���g���|�C���g
	@param[in]	hInstance		���̃��W���[���̃C���X�^���X�n���h��
	@param[in]	hPrevInstance	�O�̃C���X�^���X�B���͖��g�p
	@param[in]	lpCmdLine		�R�}���h���C���B�g�[�N�������͂���ĂȂ��A�����̕�����
	@param[in]	nCmdShow		�N�����̕\����Ԃ������Ă�B�\���Ƃ�����������
	@retval FALSE	�r���I��
*/
INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	INT		msRslt;
	MSG		msg;

	INT		iArgc;
	TCHAR	atArgv[MAX_PATH];
	LPTSTR	*pptArgs;

	INT		iCode, dMultiEna;

	LPACCEL	pstAccel;
	INT	iEntry;

#ifdef WORK_LOG_OUT
	SYSTEMTIME	stTime;
#endif
#ifdef MULTIACT_RELAY
	HWND	hWndActed;	//	�N���ς̂�̃n���h���m��
	COPYDATASTRUCT	stCopyData;
#endif

#ifdef _DEBUG
	//_CRTDBG_ALLOC_MEM_DF;		//	�w�肪�K�v�ȃt���O
	//_CRTDBG_CHECK_ALWAYS_DF;	//	���������`�F�b�N		_CRTDBG_CHECK_EVERY_128_DF
	//_CRTDBG_LEAK_CHECK_DF;		//	�I�����Ƀ��������[�N���`�F�b�N
	//_CRTDBG_DELAY_FREE_MEM_DF;	//	
	//	�����Ŏg�p����t���O���w��
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc( 1979 );
	//	���l�̓����������ԍ��B���[�N���Ă���A���̕\���̂Ƃ����
	//	�����Ă��邩��A���̊֐��Ő��l��錾���Ă��������s����ƁA
	//	���[�N����m�ۂ����Ă�Ƃ���Ŏ~�܂�
	//	���\���̕������璍��
#endif

	INITCOMMONCONTROLSEX	iccex;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC  = ICC_WIN95_CLASSES | ICC_USEREX_CLASSES | ICC_COOL_CLASSES;
	InitCommonControlsEx( &iccex );


	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, gszTitle, MAX_STRING);
	LoadString(hInstance, IDC_ORINRINEDITOR, gszWindowClass, MAX_STRING);


	//	�R�}���h���C���������m�F�E�O�͎��s�t�@�C�����A�P�ȍ~�Ɉ��������Ă�
	ZeroMemory( atArgv, sizeof(atArgv) );
	pptArgs = CommandLineToArgvW( GetCommandLine( ), &iArgc );
	//	�ŏ��̈�ڂ����L��
	if( 2 <= iArgc ){	StringCchCopy( atArgv, MAX_PATH, pptArgs[1] );	}
	LocalFree( pptArgs );

	ghMutex = NULL;

	//	���s�t�@�C���ʒu�m�F
	GetModuleFileName( hInstance, gatExePath, MAX_PATH );
	PathRemoveFileSpec( gatExePath );

	//	�ݒ�t�@�C���ʒu�m�F
	StringCchCopy( gatIniPath, MAX_PATH, gatExePath );
	PathAppend( gatIniPath, INI_FILE );

#ifdef MULTIACT_RELAY
	dMultiEna = InitParamValue( INIT_LOAD, VL_MULTI_ACT_E, 0 );
	if( !(dMultiEna) )	//	���d�N���L���Ȃ�~���[�e�b�N�X�`�F�b�N�s�v
	{
#endif

		//	�t���A�N�Z�X
		SECURITY_DESCRIPTOR	stSeqDes;
		SECURITY_ATTRIBUTES	secAttribute;
		InitializeSecurityDescriptor( &stSeqDes, SECURITY_DESCRIPTOR_REVISION );
		SetSecurityDescriptorDacl( &stSeqDes, TRUE, 0, FALSE );
		secAttribute.nLength				= sizeof( secAttribute );
		secAttribute.lpSecurityDescriptor	= &stSeqDes;
		secAttribute.bInheritHandle			= TRUE; 

		//	���d�N���h�~
		ghMutex = CreateMutex( &secAttribute, TRUE, TEXT("OrinrinEditor") );	//	���łɋN�����Ă��邩����
		if( !(ghMutex) )
		{
			//	�I�[�v�����s�A�Ƃ肠�����N��
		}
		else if( GetLastError() == ERROR_ALREADY_EXISTS )	//	���łɋN�����Ă���
		{
	#ifdef MULTIACT_RELAY
			hWndActed = FindWindow( gszWindowClass, NULL );
			SetForegroundWindow( hWndActed );	//	��ɋN���̂���t�H�A�O���E���h�ɂ���

			//	�R�}���h�����i�t�@�C�����j��������A�t�@�C�����𑗂�
			if( 0 != atArgv[0] )
			{
				stCopyData.dwData = 1;
				stCopyData.cbData = sizeof(atArgv);
				stCopyData.lpData = atArgv;

				SendMessage( hWndActed, WM_COPYDATA, NULL, (LPARAM)(&stCopyData) );
			}
	#else
			MessageBox( NULL, TEXT("�߂ɃA�v���͋N�����Ă��I"), TEXT("���ӂ���̂��m�点"), MB_OK|MB_ICONINFORMATION );
	#endif
			ReleaseMutex( ghMutex );
			CloseHandle( ghMutex );
			return 0;
		}
#ifdef MULTIACT_RELAY
	}
#endif

	InitWndwClass( hInstance );

	DocBackupDirectoryInit( gatExePath );
	FrameInitialise( gatExePath, hInstance );
	MoziInitialise( gatExePath, hInstance );

	AacBackupDirectoryInit( gatExePath );

	gbUniPad = 0;

	gbDockTmplView = TRUE;

	gbDummy = 0;

#ifdef BIG_TEXT_SEPARATE
	gbSeparateMod = FALSE;	//	�����Ǎ������^�u�ɕύX�}�[�N�K�v
#endif

#ifdef WORK_LOG_OUT
	iCode = InitParamValue( INIT_LOAD, VL_WORKLOG, 0 );
	if( iCode )
	{
		ghLogFlie = CreateFile( TEXT("LogFile.txt"), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
		if( INVALID_HANDLE_VALUE != ghLogFlie )
		{
			SetFilePointer( ghLogFlie, 0, NULL, FILE_END );

			GetLocalTime( &stTime );
			TRACE( TEXT("LOG START [%u/%02u/%02u %02u:%02u:%02u]"), stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond );
		}
	}
	else
	{
		ghLogFlie = INVALID_HANDLE_VALUE;
	}
#endif


	iCode = InitParamValue( INIT_LOAD, VL_CLASHCOVER, 0 );
	if( iCode )
	{
		TRACE( TEXT("�ُ�I���̉\���A��") );
		iCode = MessageBox( NULL, TEXT("�G�f�B�^���������I���o���Ȃ������C�z�������B\r\n�o�b�N�A�b�v���c���Ă��邩������Ȃ�����A��Ɋm�F���Ă݂āI\r\n���̂܂܋N�����Ă����������H�@�u�������v��I�ԂƁA�����ŏI�������B"), TEXT("���߂��"), MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2 );
		if( IDNO == iCode ){	return 0;	}
	}
	InitParamValue( INIT_SAVE, VL_CLASHCOVER, 1 );

	ViewingFontNameLoad(  );	//	�t�H���g���m��

	// �A�v���P�[�V�����̏����������s���܂�:
	if( !InitInstance( hInstance, nCmdShow , atArgv ) ){	return FALSE;	}

	CntxEditInitialise( gatExePath, hInstance );	//	�������̌�łȂ��Ƃ�����

	VertInitialise( gatExePath, hInstance );

	RegisterHotKey( ghMainWnd, IDHK_THREAD_DROP, MOD_CONTROL | MOD_SHIFT, VK_D );

	//	�ǂݍ��ރe�[�u���ɒ���
	pstAccel = AccelKeyTableLoadAlloc( &iEntry );
	AccelKeyTableCreate( pstAccel, iEntry );
	ToolBarInfoChange( pstAccel, iEntry );	//	�c�[���o�[�̃c�[���`�b�v�e�L�X�g�ɂ��L�[��\������
	FREE( pstAccel );

#ifdef TODAY_HINT_STYLE
	TodayHintPopup( ghMainWnd, ghInst, gatExePath );
#endif


#ifdef PLUGIN_ENABLE
	//---------------------------------------
	//[_16in] Add - 2012/04/30
	//	�v���O�C������������

	// �v���O�C���֓n���p�����[�^�̐ݒ�
	// �E�B���h�E�n���h��
	plugin::PluginInputParam	param;
	param.hMainWnd    = ghMainWnd;
	param.hMlutAAWnd  = ghMaaWnd;
	param.hFileTabWnd = ghFileTabWnd;
	param.hViewWnd    = ghViewWnd;

	// ���j���[�n���h��
	param.hMainMenu   = ghMenu;

	// �v���O�C���̏�����
	plugin::InitializePlugin( param );
	plugin::InitializePluginFileListAsync( ghMainWnd );

	//---------------------------------------
#endif


	//	���C�����b�Z�[�W���[�v
	for(;;)
	{
		msRslt = GetMessage( &msg, NULL, 0, 0 );
		if( 1 != msRslt )	break;

#ifdef FIND_STRINGS	//	�����񌟍��_�C�����O
		if( ghFindDlg )
		{	//�g�b�v�ɗ��Ă邩�ǂ������f����
			if( ghFindDlg == GetForegroundWindow(  ) )
			{
				if( TranslateAccelerator( ghFindDlg, ghAccelTable, &msg ) )	continue;
				if( IsDialogMessage( ghFindDlg, &msg ) )	continue;
			}
		}
#endif
		//	MAA�����_�C�����O
		if( ghMaaFindDlg )
		{	//�g�b�v�ɗ��Ă邩�ǂ������f����
			if( ghMaaFindDlg == GetForegroundWindow(  ) )
			{
				if( TranslateAccelerator( ghMaaFindDlg, ghAccelTable, &msg ) )	continue;
				if( IsDialogMessage( ghMaaFindDlg, &msg ) )	continue;
			}
		}

		if( !TranslateAccelerator( msg.hwnd, ghAccelTable, &msg ) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

#ifdef PLUGIN_ENABLE
	//---------------------------------------------------------------------
	//[_16in] Add - 2012/05/01
	//
	//-- �v���O�C���̔p������
	plugin::FinalizePluginList( gPluginList );
	plugin::FinalizePlugin();

	//---------------------------------------------------------------------
#endif


	InitParamValue( INIT_SAVE, VL_CLASHCOVER, 0 );

	UnregisterHotKey( ghMainWnd, IDHK_THREAD_DROP );

	if( ghMutex )
	{
		ReleaseMutex( ghMutex );
		CloseHandle( ghMutex );
	}

	TRACE( TEXT("Program Terminate") );

#ifdef WORK_LOG_OUT
	if( INVALID_HANDLE_VALUE != ghLogFlie )
	{
		SetEndOfFile( ghLogFlie );	
		CloseHandle( ghLogFlie );	
	}
#endif

	return (int)msg.wParam;
}
//-------------------------------------------------------------------------------------------------

LPTSTR ExePathGet( VOID )
{
	return gatExePath;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�N���X����
	���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
	Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
	�A�v���P�[�V�������A�֘A�t����ꂽ�������`���̏������A�C�R����
	�擾�ł���悤�ɂ���ɂ́A���̊֐����Ăяo���Ă��������B
	@param[in]	hInstance	���̃��W���[���̃C���X�^���X�n���h��
	@return �o�^�����N���X�A�g��
*/
ATOM InitWndwClass( HINSTANCE hInstance )
{
	WNDCLASSEX	wcex;

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ORINRINEDITOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ORINRINEDITOR);
	wcex.lpszClassName	= gszWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
//-------------------------------------------------------------------------------------------------

/*!
	�C���X�^���X�n���h����ۑ����āA���C���E�B���h�E���쐬���܂��B
	���̔����ŁA�O���[�o���ϐ��ŃC���X�^���X�n���h����ۑ����A
	���C���v���O�����E�B���h�E���쐬����ѕ\�����܂��B
	@param[in]	hInstance	�C���X�^���X�n���h��
	@param[in]	nCmdShow	�N�����̕\�����
	@param[in]	ptArgv		�R�}���h���C���œn���ꂽ�t�@�C�����E������΂O�N������ԁENULL�ł͂Ȃ�
*/
BOOL InitInstance( HINSTANCE hInstance, INT nCmdShow, LPTSTR ptArgv )
{
	HWND	hWnd;
	RECT	rect, wnRect;
	INT		isMaxim = 0, sptBuf = PLIST_DOCK;
//	DWORD	dwStyle;

	HMENU	hSubMenu;



	ghInst = hInstance;	//	�O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

	SplitBarClass( hInstance );	//	�X�v���b�g�o�[�̏���

	isMaxim = InitParamValue( INIT_LOAD, VL_MAXIMISED, 0 );

	InitWindowPos( INIT_LOAD, WDP_MVIEW, &rect );
	if( 0 == rect.right || 0 == rect.bottom )	//	���������O�̓f�[�^����
	{
		hWnd = GetDesktopWindow( );
		GetWindowRect( hWnd, &rect );
		rect.left   = ( rect.right  - W_WIDTH ) / 3;	//	�����
		rect.top    = ( rect.bottom - W_HEIGHT ) / 2;
		rect.right  = W_WIDTH;
		rect.bottom = W_HEIGHT;
		InitWindowPos( INIT_SAVE, WDP_MVIEW, &rect );
	}

	hWnd = CreateWindowEx( 0, gszWindowClass, gszTitle, WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right, rect.bottom, NULL, NULL, hInstance, NULL);

	if( !hWnd ){	return FALSE;	}

	gbUniPad      = InitParamValue( INIT_LOAD, VL_USE_UNICODE,  1 );	//	���j�R�󔒎g��

	gbUniRadixHex = InitParamValue( INIT_LOAD, VL_UNIRADIX_HEX, 1 );	//	���j�R�\���P�U�i��

	gdBUInterval  = InitParamValue( INIT_LOAD, VL_BACKUP_INTVL, 3 );	//	�o�b�N�A�b�v�R����
	gbAutoBUmsg   = InitParamValue( INIT_LOAD, VL_BACKUP_MSGON, 1 );	//	�o�b�N�A�b�v���b�Z�[�W�\��
	gbCrLfCode    = InitParamValue( INIT_LOAD, VL_CRLF_CODE, 0 );		//	������Ή��s

	gbSaveMsgOn   = InitParamValue( INIT_LOAD, VL_SAVE_MSGON, 1 );		//	�ۑ����b�Z�[�W�\��

	gbTmpltDock   = InitParamValue( INIT_LOAD, VL_PLS_LN_DOCK,  1 );	//	��s�u���V�e���v���ňꗗ��������

	gdPageByteMax = InitParamValue( INIT_LOAD, VL_PAGEBYTE_MAX, PAGE_BYTE_MAX );	//	�ő�o�C�g���S�O�X�U

#ifdef SPMOZI_ENCODE
	gbSpMoziEnc   = InitParamValue( INIT_LOAD, VL_SPMOZI_ENC, 0 );	//	�@��ˑ������𐔒l�Q�ƃR�s�[����
#endif

	ghMainWnd = hWnd;

	//	�V�X�e�����j���[�ɋ@�\�ǉ�
	hSubMenu = GetSystemMenu( hWnd, FALSE );
	InsertMenu( hSubMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, 0 );
	InsertMenu( hSubMenu, 0, MF_BYPOSITION, IDM_POSITION_RESET, TEXT("���z�u���Z�b�g") );

	ghMenu = GetMenu( hWnd );

	//	���j���[�Ƀc���[�\�z�͂���Ȃ��̂ŃJ�b�g
	hSubMenu = GetSubMenu( ghMenu , 0 );	//	�t�@�C��
	DeleteMenu( hSubMenu, IDM_TREE_RECONSTRUCT, MF_BYCOMMAND );
#ifndef ACCELERATOR_EDIT
	DeleteMenu( hSubMenu, IDM_ACCELKEY_EDIT_DLG_OPEN, MF_BYCOMMAND );
#endif

	hSubMenu = GetSubMenu( ghMenu , 1 );	//	�ҏW
#ifndef SEARCH_HIGHLIGHT
	DeleteMenu( hSubMenu, IDM_FIND_HIGHLIGHT_OFF, MF_BYCOMMAND );
#endif
#ifndef FIND_STRINGS
	DeleteMenu( hSubMenu, IDM_FIND_DLG_OPEN, MF_BYCOMMAND );
//	DeleteMenu( hSubMenu, 19, MF_BYPOSITION );	//	�폜���Ԓ���
#endif

//	hSubMenu = GetSubMenu( ghMenu , 2 );	//	�}��

	hSubMenu = GetSubMenu( ghMenu , 4 );	//	�\��
	if( gbTmpltDock )
	{
		DeleteMenu( hSubMenu, IDM_PAGELIST_VIEW, MF_BYCOMMAND );
		DeleteMenu( hSubMenu, IDM_LINE_TEMPLATE, MF_BYCOMMAND );
		DeleteMenu( hSubMenu, IDM_BRUSH_PALETTE, MF_BYCOMMAND );
	}

	AaFontCreate( 1 );	//	

	AppClientAreaCalc( &rect  );	//	

	SqnSetting(  );

	if( gbTmpltDock )	//	�h�b�L���O�ݒ�ł��邩
	{
		//	�ݒ肩��X�v���b�g�o�[�̈ʒu����������
		grdSplitPos = InitParamValue( INIT_LOAD, VL_MAIN_SPLIT, PLIST_DOCK );
		if( grdSplitPos < SPLITBAR_WIDTH || rect.right <= grdSplitPos ){	grdSplitPos = PLIST_DOCK;	}
		sptBuf = grdSplitPos;

		ghMainSplitWnd = SplitBarCreate( hInstance, hWnd, rect.right - grdSplitPos, rect.top, rect.bottom );
	}
	else{	ghMainSplitWnd = NULL;	grdSplitPos = 0;	}


	ghPgVwWnd = PageListInitialise( hInstance, hWnd, &rect );	//	�ňꗗ

	ghLnTmplWnd = LineTmpleInitialise( hInstance, hWnd, &rect );	//	��s�e���v��

	ViewInitialise( hInstance, hWnd, &rect, ptArgv );	//	�ҏW�r���[�쐬
	ViewMaaItemsModeSet( InitParamValue( INIT_LOAD, VL_MAA_LCLICK, MAA_INSERT ), InitParamValue( INIT_LOAD, VL_MAA_MCLICK, MAA_INSERT ) );

	LayerBoxInitialise( hInstance, &rect );	//	���C���{�b�N�X�쐬
	LayerBoxAlphaSet( InitParamValue( INIT_LOAD, VL_LAYER_TRANS, 192 ) );

	UniDlgInitialise( hWnd, TRUE );

	UserDefInitialise( hWnd, TRUE );

#ifdef ENABLE_PREVIEW
	PreviewInitialise( hInstance, hWnd );
#endif

	FrameNameModifyMenu( hWnd );

	TraceInitialise( hWnd, TRUE );

	OpenHistoryInitialise( hWnd );

	OpenProfileInitialise( hWnd );

	DocInverseInit( TRUE );

#ifdef FIND_STRINGS
	FindDialogueOpen( NULL, NULL );
#endif

#ifdef USE_HOVERTIP
	HoverTipInitialise( hInstance, hWnd );
#endif

	SetFocus( ghViewWnd );

	if( 1 <= gdBUInterval )
	{
		SetTimer( hWnd, IDT_BACKUP_TIMER, (gdBUInterval * 60000), NULL );
	}

	ghBrTmplWnd = BrushTmpleInitialise( hInstance, hWnd, &rect, ghMaaWnd );	//	�u���V��


	ghMaaWnd = MaaTmpltInitialise( hInstance, hWnd, &wnRect );

	MenuItemCheckOnOff( IDM_MAATMPLE_VIEW, MaaViewToggle( 0 ) );	//	�����s�e���v�����̕\���E��\���̃`�F�L

	MenuItemCheckOnOff( IDM_UNICODE_TOGGLE , gbUniPad );	 //	���j�R�[�hON/OFF�̃`�F�L

	DraughtInitialise( hInstance, hWnd );

	if( isMaxim )
	{
		ShowWindow( hWnd, SW_MAXIMIZE );
		AppClientAreaCalc( &rect  );	//	
		if( ghMainSplitWnd )
		{
			grdSplitPos = sptBuf;
			SetWindowPos( ghMainSplitWnd, HWND_TOP, rect.right - grdSplitPos, rect.top, 0, 0, SWP_NOSIZE );
		}
		Cls_OnSize( hWnd, SIZE_MAXIMIZED, rect.right, rect.top );
	}
	else{	ShowWindow( hWnd, nCmdShow );	}

	UpdateWindow( hWnd );

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

#ifdef ACCELERATOR_EDIT

/*!
	�A�N�Z���e�[�u�����_�C�i�~�b�N�Ɋm��
	@param[out]	piEntry	�m�ۂ����G���g������߂�
	@return		�m�ۂ����e�[�u���\���̂̃|�C���^�[�Efree�ɒ���
*/
LPACCEL AccelKeyTableGetAlloc( LPINT piEntry )
{
	INT	iItems;
	LPACCEL	pstAccel = NULL;

	if( !(piEntry) )	return NULL;
	*piEntry = 0;

	//	�܂����m��
	iItems = CopyAcceleratorTable( ghAccelTable, NULL, 0 );
	if( 0 >= iItems )	return NULL;

	pstAccel = (LPACCEL)malloc( iItems * sizeof(ACCEL) );
	if( !(pstAccel) )	return NULL;

	iItems = CopyAcceleratorTable( ghAccelTable, pstAccel, iItems );

	*piEntry = iItems;

	return pstAccel;
}
//-------------------------------------------------------------------------------------------------

/*!
	�\���̃e�[�u�����󂯎���āA������A�N�Z���n���h���ɂ���ENULL�Ȃ�Resource����
	@param[in]	pstAccel	���e�e�[�u���E��������NULL
	@param[in]	iEntry		�e�[�u���̃G���g����
	@return	�A�N�Z���n���h��
*/
HACCEL AccelKeyTableCreate( LPACCEL pstAccel, INT iEntry )
{
	HACCEL	hAccel;
	
	if( pstAccel )
	{
		DestroyAcceleratorTable( ghAccelTable );	//	�O�̃��c�Ԃ��󂵂�

		hAccel = CreateAcceleratorTable( pstAccel, iEntry );
	}
	else
	{
		hAccel = LoadAccelerators( ghInst, MAKEINTRESOURCE(IDC_ORINRINEDITOR) );
	}

	ghAccelTable = hAccel;	//	���ϐ��ɂ����E�{���͂������������͗ǂ��Ȃ��Ǝv���

	AccelKeyMenuRewrite( ghMainWnd, pstAccel, iEntry );

	return hAccel;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	�^�C�g���o�[��ύX����
	@param[in]	ptText	�^�C�g���ɂ������镶����ENULL�ŏI����ĂȂ��ƍ����ڂɂ�����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT AppTitleChange( LPTSTR ptText )
{
	LPTSTR	ptName;
	TCHAR	atBuff[MAX_PATH];

	if( ptText )
	{
		if( NULL != ptText[0] ){	ptName = PathFindFileName( ptText );	}
		else{						ptName = NAMELESS_DUMMY;	}
		StringCchPrintf( atBuff, MAX_PATH, TEXT("%s - %s"), gszTitle, ptName );
	}
	else
	{
		StringCchCopy( atBuff, MAX_PATH, gszTitle );
	}

	SetWindowText( ghMainWnd, atBuff );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�^�C�g���o�[�Ƀg���X���[�h�\�L��Ђ������������
	@param[in]	bMode	��O�g���X���[�h�J�n�@�O�I���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT AppTitleTrace( UINT bMode )
{
	static	TCHAR	atOrig[MAX_PATH];
	TCHAR	atBuff[MAX_PATH];

	if( bMode )
	{
		GetWindowText( ghMainWnd, atOrig, MAX_PATH );
		StringCchPrintf( atBuff, MAX_PATH, TEXT("%s [�g���X���[�h]"), atOrig );
		SetWindowText( ghMainWnd, atBuff );
	}
	else
	{
		if( 0 != atOrig[0] ){	SetWindowText( ghMainWnd , atOrig );	}
		else{	SetWindowText( ghMainWnd, gszTitle );	}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�o�[�W�������{�b�N�X�̃��b�Z�[�W�n���h���ł��B
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_INITDIALOG:
			SetDlgItemText( hDlg, IDE_ABOUT_DISP, gcatLicense );
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���̃E�C���h�E�v���V�[�W��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HMENU	hSubMenu;
#ifdef NDEBUG
	INT_PTR	iRslt;
#endif

	switch( message )
	{
		HANDLE_MSG( hWnd, WM_CREATE,		Cls_OnCreate );		//	��ʂ̍\���p�[�c�����B�{�^���Ƃ�
		HANDLE_MSG( hWnd, WM_PAINT,			Cls_OnPaint );		//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG( hWnd, WM_COMMAND,		Cls_OnCommand );	//	�{�^�������ꂽ�Ƃ��̃R�}���h����
		HANDLE_MSG( hWnd, WM_DESTROY,		Cls_OnDestroy );	//	�\�t�g�I�����̏���
		HANDLE_MSG( hWnd, WM_SIZE,			Cls_OnSize );		//	�T�C�Y�ύX���ꂽ��
		HANDLE_MSG( hWnd, WM_MOVE,			Cls_OnMove );		//	�ʒu�ύX���ꂽ��
		HANDLE_MSG( hWnd, WM_DROPFILES,		Cls_OnDropFiles );	//	�h���b�O���h���b�y
		HANDLE_MSG( hWnd, WM_ACTIVATE,		Cls_OnActivate );	//	�A�N�e�B�u�ɂȂ�����͂��ꂽ��
		HANDLE_MSG( hWnd, WM_NOTIFY,		Cls_OnNotify  );	//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_TIMER,			Cls_OnTimer );		//	�^�C�}
		HANDLE_MSG( hWnd, WM_CONTEXTMENU,	Cls_OnContextMenu );//	�E�N���߂ɂ�
		HANDLE_MSG( hWnd, WM_HOTKEY,		Cls_OnHotKey  );	//	�z�b�g�L�[�������ꂽ

		HANDLE_MSG( hWnd, WM_DRAWITEM,		Cls_OnDrawItem );	//	�I�[�i�[�h���[
#ifdef MULTIACT_RELAY
		HANDLE_MSG( hWnd, WM_COPYDATA,		Cls_OnCopyData );
#endif
		HANDLE_MSG( hWnd, WM_KEYDOWN,		Evw_OnKey );		//	
		HANDLE_MSG( hWnd, WM_KEYUP,			Evw_OnKey );		//	
		HANDLE_MSG( hWnd, WM_CHAR,			Evw_OnChar );		//	
		HANDLE_MSG( hWnd, WM_MOUSEWHEEL,	Evw_OnMouseWheel );	//	

		HANDLE_MSG( hWnd, WM_WINDOWPOSCHANGING,	Cls_OnWindowPosChanging );	

		case WM_MBUTTONUP:	TRACE( TEXT("MIDDLE  UP") );	break;

		case WM_SYSCOMMAND:
			if( IDM_POSITION_RESET == LOWORD(wParam) )
			{
				WindowPositionReset( hWnd );
				return 0;
			}
			break;

#ifdef NDEBUG
		case WM_CLOSE:
			iRslt = DocFileCloseCheck( hWnd, TRUE );
			if( !(iRslt)  ){	return FALSE;	}
			break;
#endif

#ifdef USE_NOTIFYICON
		//	�^�X�N�g���C�̃A�C�R���ŃG�x���c����
		case WMP_TRAYNOTIFYICON:
			TaskTrayIconEvent( hWnd, (UINT)wParam, (UINT)lParam );
			return 0;
#endif

		case WMP_BRUSH_TOGGLE:
			hSubMenu = GetSubMenu( ghMenu, 1 );
			CheckMenuItem( hSubMenu, IDM_BRUSH_STYLE, wParam ? MF_CHECKED : MF_UNCHECKED );
			return 0;

		case WMP_PREVIEW_CLOSE:
			DestroyCaret(  );
			ViewFocusSet(  );
			ViewShowCaret(  );
			return 0;

		default:	break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	�������ԂȂ�����Ă΂��
	@param[in]	hWnd			�E�C���h�E�n���h��
	@param[in]	state			���
	@param[in]	hWndActDeact	�������Ԗ��͔񂠂����ԂɂȂ��ENULL�ł���
	@param[in]	fMinimized		�ŏ����Ȃ�NULL
*/
VOID Cls_OnActivate( HWND hWnd, UINT state, HWND hWndActDeact, BOOL fMinimized )
{
	LONG_PTR	rdExStyle;
	HWND		hWorkWnd;
//	BOOLEAN		bRslt;

/*
WA_INACTIVE		0	�E�B���h�E����A�N�e�B�u�ɂȂ�܂��B
WA_ACTIVE		1	�}�E�X���N���b�N����ȊO�̕��@ (�E�B���h�E�̑I���ɃL�[�{�[�h �C���^�[�t�F�C�X���g�p������@�Ȃ�) �ŁA�E�B���h�E���A�N�e�B�u�ɂȂ�܂��B
WA_CLICKACTIVE	2	�}�E�X�N���b�N�ɂ���āA�E�B���h�E���A�N�e�B�u�ɂȂ�܂��B
*/
	TRACE( TEXT("MAIN ACTIVATE STATE[%u] HWND[%X] MIN[%u]"), state, hWndActDeact, fMinimized );

	if( WA_INACTIVE == state )
	{
	}
	else
	{

		rdExStyle = GetWindowLongPtr( ghPgVwWnd, GWL_EXSTYLE );
		if( WS_EX_TOPMOST & rdExStyle )	hWorkWnd = HWND_TOPMOST;
		else							hWorkWnd = hWnd;
		SetWindowPos( ghPgVwWnd, hWorkWnd, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE );

		if( InitParamValue( INIT_LOAD, VL_MAA_TOPMOST, 1 ) )	//	�l�`�`��
		{
			rdExStyle = GetWindowLongPtr( ghMaaWnd, GWL_EXSTYLE );	//	���O�̏����͕s�v
			if( WS_EX_TOPMOST & rdExStyle )	hWorkWnd = HWND_TOPMOST;
			else							hWorkWnd = hWnd;
			SetWindowPos( ghMaaWnd, hWorkWnd, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE );
		}
		else	//	�����I�ɂn�e�e�H
		{
			ShowWindow( ghMaaWnd, SW_HIDE );
		}

		if( IsWindowVisible(ghLnTmplWnd ) )
		{
			rdExStyle = GetWindowLongPtr( ghLnTmplWnd, GWL_EXSTYLE );
			if( WS_EX_TOPMOST & rdExStyle )	hWorkWnd = HWND_TOPMOST;
			else							hWorkWnd = hWnd;
			SetWindowPos( ghLnTmplWnd, hWorkWnd, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE );
		}

		if( IsWindowVisible(ghBrTmplWnd ) )
		{
			rdExStyle = GetWindowLongPtr( ghBrTmplWnd, GWL_EXSTYLE );
			if( WS_EX_TOPMOST & rdExStyle )	hWorkWnd = HWND_TOPMOST;
			else							hWorkWnd = hWnd;
			SetWindowPos( ghBrTmplWnd, hWorkWnd, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE );
		}

		ViewFocusSet(  );
	}

	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	���C���̃N���G�C�g�B
	@param[in]	hWnd			�e�E�C���h�E�̃n���h��
	@param[in]	lpCreateStruct	�A�v���P�[�V�����̏��������e
	@return	TRUE	���ɂȂ�
*/
BOOLEAN Cls_OnCreate( HWND hWnd, LPCREATESTRUCT lpCreateStruct )
{
	RECT	clRect, tbRect;

	RECT	tiRect;
	TCITEM	stTcItem;
	TTTOOLINFO	stToolInfo;

	HINSTANCE lcInst = lpCreateStruct->hInstance;	//	�󂯎������������񂩂�A�C���X�^���X�n���h�����Ђ��ς�


	DragAcceptFiles( hWnd, TRUE );

	GetClientRect( hWnd, &clRect );

#ifdef USE_NOTIFYICON
	NOTIFYICONDATA	nid;
	//	�^�X�N�g���C�A�C�R���ƃo���[���w���v
	ZeroMemory( &nid, sizeof(NOTIFYICONDATA) );
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd   = hWnd;
	nid.uID    = 0;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WMP_TRAYNOTIFYICON;
	nid.hIcon  = LoadIcon( lcInst, MAKEINTRESOURCE(IDI_ORINRINEDITOR) );
	StringCchCopy( nid.szTip, 128, gszTitle );
	Shell_NotifyIcon( NIM_ADD, &nid );
#endif

//�c�[���o�[�쐬
	ToolBarCreate( hWnd, lcInst );

	ghNameFont = CreateFont( 12, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, TEXT("MS UI Gothic") );

//�}���`�t�@�C���I�[�|��
	ghFileTabWnd = CreateWindowEx( 0, WC_TABCONTROL, TEXT("filetab"), WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | TCS_SINGLELINE, 0, 0, clRect.right, 0, hWnd, (HMENU)IDTB_MULTIFILE, lcInst, NULL );
	SetWindowFont( ghFileTabWnd, ghNameFont, FALSE );

	//	�T�u�N���X
	gpfOriginFileTabProc = SubclassWindow( ghFileTabWnd, gpfFileTabProc );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask    = TCIF_TEXT;
	stTcItem.pszText = NAMELESS_DUMMY;
	TabCtrl_InsertItem( ghFileTabWnd, 0, &stTcItem );

	ToolBarSizeGet( &tbRect );
	TabCtrl_GetItemRect( ghFileTabWnd, 1, &tiRect );
	tiRect.bottom  += tiRect.top;
	MoveWindow( ghFileTabWnd, 0, tbRect.bottom, clRect.right, tiRect.bottom, TRUE );

	//	�c�[���`�b�v�E�t�@�C���p�X�\���`
	ghFileTabTip = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, ghFileTabWnd, NULL, lcInst, NULL );
	//	�c�[���`�b�v���R�[���o�b�N�Ŋ���t��
	ZeroMemory( &stToolInfo, sizeof(TTTOOLINFO) );
	GetClientRect( ghFileTabWnd, &stToolInfo.rect );
	stToolInfo.cbSize   = sizeof(TTTOOLINFO);
	stToolInfo.uFlags   = TTF_SUBCLASS;
	stToolInfo.hinst    = NULL;	//	
	stToolInfo.hwnd     = ghFileTabWnd;
	stToolInfo.uId      = IDTT_TILETAB_TIP;
	stToolInfo.lpszText = LPSTR_TEXTCALLBACK;	//	�R�����w�肷��ƃR�[���o�b�N�ɂȂ�
	SendMessage( ghFileTabTip, TTM_ADDTOOL, 0, (LPARAM)&stToolInfo );
	SendMessage( ghFileTabTip, TTM_SETMAXTIPWIDTH, 0, 0 );	//	�`�b�v�̕��B�O�ݒ�ł����B���ꂵ�Ƃ��Ȃ��Ɖ��s����Ȃ�


	ghStsRedBrush = CreateSolidBrush( 0xFF );

	ghStsBarWnd = CreateStatusWindow( WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP, TEXT(""), hWnd, IDSB_VIEW_STATUS_BAR );
	SendMessage( ghStsBarWnd, SB_SETPARTS, (WPARAM)SB_ITEMS, (LPARAM)(LPINT)gadStsBarSize );

	StatusBar_SetText( ghStsBarWnd, 1 , TEXT("") );

//	StatusBar_SetText( ghStsBarWnd, (SB_BYTECNT | SBT_OWNERDRAW) , 0 );	//	�I�[�i�[�h���[

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C����COMMAND���b�Z�[�W�̎󂯎��B�{�^�������ꂽ�Ƃ��Ŕ���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	id			���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq	LOWORD(wParam)
	@param[in]	hWndCtl		���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��	lParam
	@param[in]	codeNotify	�ʒm���b�Z�[�W	HIWORD(wParam)
	@return		�Ȃ�
*/
VOID Cls_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
#ifdef NDEBUG
	 INT	iRslt;
#endif
	//	�L�[�{�[�h�V���[�g�J�b�g�́A���̂Ƃ�ACTIVE�ȃE�C���h�E�ɍs��

	switch( id )
	{
		case  IDM_ABOUT:	DialogBox( ghInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About );	return;

		case IDM_EXIT:	//	WM_CLOSE�Ƃ̐������ɒ��ӃZ��
#ifdef NDEBUG
			iRslt = DocFileCloseCheck( hWnd, TRUE );
			if( iRslt ){	DestroyWindow( hWnd );	}
#else
			DestroyWindow( hWnd );
#endif
			return;
	}

	OperationOnCommand( hWnd, id, hWndCtl, codeNotify );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Cls_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;

	hdc = BeginPaint( hWnd, &ps );

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�^�C�}�̃A��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	id		�^�C�}�h�c
	@return		����
*/
VOID Cls_OnTimer( HWND hWnd, UINT id )
{
	if( IDT_BACKUP_TIMER != id )	return;
	//	�^�C�}�������璍��

	DocFileBackup( hWnd );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�B���h�E�̃T�C�Y�ύX����������O�ɑ����Ă���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pstWpos	�V�����ʒu�Ƒ傫���������Ă�
	@return		����Message������������O
*/
BOOL Cls_OnWindowPosChanging( HWND hWnd, LPWINDOWPOS pstWpos )
{
	RECT	winPos;

	GetWindowRect( hWnd, &winPos );	//	�T�C�Y�ύX�O�̂��Q�b�c�ł���
	winPos.right  = winPos.right - winPos.left;
	winPos.bottom = winPos.bottom - winPos.top;	//	����ʒu�ƁA�������ɂ��Ă���

	TRACE( TEXT("MAIN WM_WINDOWPOSCHANGING [%d %d %d %d][%d %d %d %d]"), winPos.left, winPos.top, winPos.right, winPos.bottom, pstWpos->x, pstWpos->y, pstWpos->cx, pstWpos->cy );

	giResizeWide = pstWpos->cx - winPos.right;	//	�v���}�C�������ӁB�{�ŕ��L�����Ă�
	//	�������Ƃ��Ƃ�

	//	���̂Ƃ���Ӗ��̖�������

	return TRUE;
}
//-------------------------------------------------------------------------------------------------


/*!
	�E�C���h�E���T�C�Y�ύX���ꂽ�Ƃ�
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	state	�ύX�̏�ԁESIZE_MINIMIZED �Ƃ�
	@param[in]	cx		�N���C�����g�w�T�C�Y
	@param[in]	cy		�N���C�����g�x�T�C�Y
	@return		����
*/
VOID Cls_OnSize( HWND hWnd, UINT state, INT cx, INT cy )
{
	static  UINT	ccState;

	RECT	rect;

	if( SIZE_MINIMIZED == state )	//	�ŏ�����
	{
		ShowWindow( ghMaaWnd, SW_HIDE );
		if( !(gbTmpltDock) )
		{
			ShowWindow( ghPgVwWnd, SW_HIDE );
			ShowWindow( ghLnTmplWnd, SW_HIDE );
			ShowWindow( ghBrTmplWnd, SW_HIDE );
		}
		ccState = SIZE_MINIMIZED;

		return;
	}

	if( SIZE_MINIMIZED == ccState &&  ccState != state )	//	�ŏ������畜�A
	{
		ShowWindow( ghMaaWnd, SW_SHOW );
		if( !(gbTmpltDock) )
		{
			ShowWindow( ghPgVwWnd,   SW_SHOW );
			ShowWindow( ghLnTmplWnd, SW_SHOW );
			ShowWindow( ghBrTmplWnd, SW_SHOW );
		}
		ccState = SIZE_RESTORED;
	}

	if( SIZE_MAXIMIZED == state )	//	�ő剻��
	{
#ifndef SPLIT_BAR_POS_FIX
		if( ghMainSplitWnd )
		{
			AppClientAreaCalc( &rect  );	//	�E�ɕ����Ĉړ�
			SetWindowPos( ghMainSplitWnd, HWND_TOP, rect.right - grdSplitPos, rect.top, 0, 0, SWP_NOSIZE );
		}
#endif
		ccState = SIZE_MAXIMIZED;
	}

	if( SIZE_RESTORED == state &&  SIZE_MAXIMIZED == ccState )	//	�ő剻���畜�A��
	{
		if( !(IsZoomed( hWnd ) ) )	//	�܂��ő剻���Ȃ�A�X�v���b�g�o�[�����̂͂�
		{
#ifndef SPLIT_BAR_POS_FIX
			if( ghMainSplitWnd )
			{
				AppClientAreaCalc( &rect  );	//	�E�ɕ����Ĉړ�
				SetWindowPos( ghMainSplitWnd, HWND_TOP, rect.right - grdSplitPos, rect.top, 0, 0, SWP_NOSIZE );
			}
#endif
			ccState = SIZE_RESTORED;
		}
	}

	MoveWindow( ghStsBarWnd, 0, 0, 0, 0, TRUE );	//	�X�e�[�^�X�o�[�͏���Ɉʒu�Ƃ����������̂ł���ł���

	ToolBarOnSize( hWnd, state, cx, cy );

	AppClientAreaCalc( &rect  );	//	�E�ɕ����Ĉړ�

#ifdef SPLIT_BAR_POS_FIX
	if( ghMainSplitWnd && (SIZE_SPLITBAR_MOVED != state) )	//	�����ŃX�v���b�g�o�[�����H	
	{
		SetWindowPos( ghMainSplitWnd, HWND_TOP, rect.right - grdSplitPos, rect.top, 0, 0, SWP_NOSIZE );
	}
#endif
	ViewSizeMove( hWnd, &rect );	//	���C�����̃T�C�Y�ύX����



	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E���ړ������Ƃ�
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	x		
	@param[in]	y		
	@return		����
*/
VOID Cls_OnMove( HWND hWnd, INT x, INT y )
{
	DWORD	dwStyle;

	dwStyle = GetWindowStyle( hWnd );
	if( dwStyle & WS_MINIMIZE )
	{
		LayerMoveFromView( hWnd, SIZE_MINIMIZED );
		FrameMoveFromView( hWnd, SIZE_MINIMIZED );
		MoziMoveFromView( hWnd, SIZE_MINIMIZED );
		VertMoveFromView( hWnd, SIZE_MINIMIZED );
	}
	else
	{
		LayerMoveFromView( hWnd, SIZE_RESTORED );
		FrameMoveFromView( hWnd, SIZE_RESTORED );
		MoziMoveFromView( hWnd, SIZE_RESTORED );
		VertMoveFromView( hWnd, SIZE_RESTORED );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�����Ƃ��ɔ����B�f�o�C�X�R���e�L�X�g�Ƃ��S���I���B
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Cls_OnDestroy( HWND hWnd )
{
	RECT	rect;
	DWORD	dwStyle;

#ifdef USE_NOTIFYICON
	NOTIFYICONDATA	nid;

	ZeroMemory( &nid, sizeof(NOTIFYICONDATA) );
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd   = hWnd;
	Shell_NotifyIcon( NIM_DELETE, &nid );
#endif

	DocInverseInit( FALSE );

#ifdef USE_HOVERTIP
	HoverTipInitialise( NULL, NULL );
#endif

	ToolBarBandInfoGet( NULL );

#ifdef ENABLE_PREVIEW
	PreviewInitialise( NULL, NULL );
#endif

	TraceInitialise( hWnd, FALSE );

	OpenHistoryInitialise( NULL );

	OpenProfileInitialise( NULL );

	BrushTmpleInitialise( NULL, NULL, NULL, NULL );

	LayerBoxInitialise( NULL, NULL );

	FrameInitialise( NULL, NULL );

	MoziInitialise( NULL, NULL );

	UniDlgInitialise( NULL, FALSE );

	VertInitialise( NULL, NULL );

	DraughtInitialise( NULL, NULL );

	CntxEditInitialise( NULL, NULL );

	//	�E�C���h�E�̏�Ԃ��m�F���āA�ŏ�����ԂȂ�L�^���Ȃ�
	dwStyle = GetWindowStyle( hWnd );
	if( !(dwStyle & WS_MINIMIZE) )
	{
		if( dwStyle & WS_MAXIMIZE ){	InitParamValue( INIT_SAVE, VL_MAXIMISED, 1 );	}
		else
		{
			//	�ő剻���Ă���L�^���Ȃ�
			GetWindowRect( ghMainWnd, &rect );
			rect.right  = rect.right  - rect.left;
			rect.bottom = rect.bottom - rect.top;
			InitWindowPos( INIT_SAVE, WDP_MVIEW, &rect );//�I�����ۑ�

			InitParamValue( INIT_SAVE, VL_MAXIMISED, 0 );
		}

		if( !(gbTmpltDock) )
		{
			GetWindowRect( ghPgVwWnd, &rect );
			rect.right  = rect.right  - rect.left;
			rect.bottom = rect.bottom - rect.top;
			InitWindowPos( INIT_SAVE, WDP_PLIST, &rect );//�I�����ۑ�

			GetWindowRect( ghLnTmplWnd, &rect );
			rect.right  = rect.right  - rect.left;
			rect.bottom = rect.bottom - rect.top;
			InitWindowPos( INIT_SAVE, WDP_LNTMPL, &rect );//�I�����ۑ�

			GetWindowRect( ghBrTmplWnd, &rect );
			rect.right  = rect.right  - rect.left;
			rect.bottom = rect.bottom - rect.top;
			InitWindowPos( INIT_SAVE, WDP_BRTMPL, &rect );//�I�����ۑ�
		}
	}


	//	MAA�̂͂��������ł���Ă�
	DestroyWindow( ghMaaWnd );

	DestroyWindow( ghPgVwWnd );


	InitMultiFileTabOpen( INIT_SAVE, 0, NULL );

	DocInitialise( FALSE );

	SetWindowFont( ghFileTabWnd, GetStockFont(DEFAULT_GUI_FONT), FALSE );

	DeleteFont( ghNameFont );

	DeleteBrush( ghStsRedBrush );

	ToolBarDestroy(  );

	DestroyWindow( ghViewWnd );
	AaFontCreate( 0 );

	PostQuitMessage( 0 );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���e�L�X�g���j���[�Ăт����A�N�V����(�v�͉E�N���b�N�j
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	hWndContext	�R���e�L�X�g�����������E�C���h�E�̃n���h��
	@param[in]	xPos		�X�N���[���w���W
	@param[in]	yPos		�X�N���[���x����
	@return		����
*/
VOID Cls_OnContextMenu( HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos )
{
	HMENU	hMenu, hSubMenu;
	UINT	dRslt;
	INT		curSel;
	INT		iCount;
	POINT	stPost;
	TCHITTESTINFO	stTcHitInfo;

	MENUITEMINFO	stMenuItemInfo;

	TCITEM	stTcItem;
	TCHAR	atText[MAX_PATH];
	UINT	cchSize;

	stPost.x = (SHORT)xPos;	//	��ʍ��W�̓}�C�i�X�����肤��
	stPost.y = (SHORT)yPos;

	TRACE( TEXT("MAIN CONTEXT[%d x %d]"), stPost.x, stPost.y );

	//���o�[�́A�o�[���̂ƍ��̎����̂Ƃ���͔�������E
	if( ToolBarOnContextMenu( hWnd, hWndContext, stPost.x, stPost.y ) ){	 return;	}

	if( SUCCEEDED( DockingTabContextMenu( hWnd, hWndContext, stPost.x, stPost.y ) ) ){	 return;	}

	//	�����^�u�ȊO�ɂ͂Ȃ�
	if( ghFileTabWnd != hWndContext ){	 return;	}

	iCount = TabCtrl_GetItemCount( ghFileTabWnd );

	hMenu = LoadMenu( GetModuleHandle(NULL), MAKEINTRESOURCE(IDM_MULTIFILE_POPUP) );
	hSubMenu = GetSubMenu( hMenu, 0 );

	stTcHitInfo.pt = stPost;
	ScreenToClient( ghFileTabWnd, &(stTcHitInfo.pt) );
	curSel = TabCtrl_HitTest( ghFileTabWnd, &stTcHitInfo );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask       = TCIF_TEXT | TCIF_PARAM;
	stTcItem.pszText    = atText;
	stTcItem.cchTextMax = MAX_PATH;
	TabCtrl_GetItem( ghFileTabWnd, curSel, &stTcItem );

	//	�E�N�������z��I������
	TabCtrl_SetCurSel( ghFileTabWnd, curSel );
	DocMultiFileSelect( stTcItem.lParam );

	StringCchCat( atText, MAX_PATH, TEXT(" �����(&Q)") );
	StringCchLength( atText, MAX_PATH, &cchSize );

	ZeroMemory( &stMenuItemInfo, sizeof(MENUITEMINFO) );
	stMenuItemInfo.cbSize     = sizeof(MENUITEMINFO);
	stMenuItemInfo.fMask      = MIIM_TYPE;
	stMenuItemInfo.fType      = MFT_STRING;
	stMenuItemInfo.cch        = cchSize;
	stMenuItemInfo.dwTypeData = atText;

	if( 1 >= iCount )	//	�c��t�@�C�����������̎��͕���Ȃ�
	{
		stMenuItemInfo.fMask |= MIIM_STATE;
		stMenuItemInfo.fState = MFS_GRAYED;
//		EnableMenuItem( hSubMenu, IDM_FILE_CLOSE, MF_BYCOMMAND | MF_GRAYED );
	}
	
	SetMenuItemInfo( hSubMenu, IDM_FILE_CLOSE, FALSE, &stMenuItemInfo );

	dRslt = TrackPopupMenu( hSubMenu, 0, stPost.x, stPost.y, 0, hWnd, NULL );

	DestroyMenu( hMenu );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���E�C���h�E�̃m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
LRESULT Cls_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	INT	curSel;
	TCHAR	atText[MAX_PATH];
	TCITEM	stTcItem;
	LPNMTTDISPINFO	pstDispInfo;
	TCHITTESTINFO	stTcHitTest;


	ToolBarOnNotify( hWnd, idFrom, pstNmhdr );	//	�c�[���o�[�̏���

	if( IDTB_DOCK_TAB == idFrom )
	{
		if( TCN_SELCHANGE == pstNmhdr->code )
		{
			curSel = TabCtrl_GetCurSel( pstNmhdr->hwndFrom );

			TRACE( TEXT("TMPL TAB sel [%d]"), curSel );

			//	��\�����͉������Ȃ�
			if( gbDockTmplView )
			{
				//	�t�H�[�J�X����ւ�
				switch( curSel )
				{
					case  0:	ShowWindow( ghLnTmplWnd , SW_SHOW );	ShowWindow( ghBrTmplWnd , SW_HIDE );	break;
					case  1:	ShowWindow( ghLnTmplWnd , SW_HIDE );	ShowWindow( ghBrTmplWnd , SW_SHOW );	break;
					default:	break;
				}
			}
		}
	}

	if( IDTB_MULTIFILE == idFrom )	//	�t�@�C���^�u�I��
	{
		if( TCN_SELCHANGE == pstNmhdr->code )
		{
			curSel = TabCtrl_GetCurSel( ghFileTabWnd );

			TRACE( TEXT("FILE TAB sel [%d]"), curSel );

			ZeroMemory( &stTcItem, sizeof(TCITEM) );
			stTcItem.mask       = TCIF_TEXT | TCIF_PARAM;
			stTcItem.pszText    = atText;
			stTcItem.cchTextMax = MAX_PATH;
			TabCtrl_GetItem( ghFileTabWnd, curSel, &stTcItem );

			TRACE( TEXT("FILE [%s] param[%d]"), atText, stTcItem.lParam );

			DocMultiFileSelect( stTcItem.lParam );
		}
	}

	if( IDTT_TILETAB_TIP == idFrom )	//	�c�[���`�b�v�̏ꍇ
	{
		if( TTN_GETDISPINFO ==  pstNmhdr->code )	//	���e�̖₢���킹��������
		{
			GetCursorPos( &(stTcHitTest.pt) );	//	�X�N���[�����W
			ScreenToClient( ghFileTabWnd, &(stTcHitTest.pt) );	//	�N���C�����g���W�ɕϊ�

			curSel = TabCtrl_HitTest( ghFileTabWnd, &stTcHitTest );
			TRACE( TEXT("FILE TAB under [%d]"), curSel );

			pstDispInfo = (LPNMTTDISPINFO)pstNmhdr;

			ZeroMemory( &(pstDispInfo->szText), sizeof(pstDispInfo->szText) );

			pstDispInfo->lpszText = DocMultiFileNameGet( curSel );
			if( !(pstDispInfo->lpszText)  )
			{
				StringCchCopy( pstDispInfo->szText, 80, TEXT(" ") );
				pstDispInfo->lpszText = pstDispInfo->szText;
			}
		}
	}

	return 0;	//	�����Ȃ��Ȃ�O��߂�
}
//-------------------------------------------------------------------------------------------------

#ifdef MULTIACT_RELAY

/*!
	�ʂɋN�����������̑��M���󂯎��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	hWndFrom	�������̃n���h��
	@param[in]	pstCopyData	�g���ł������e
*/
void Cls_OnCopyData( HWND hWnd, HWND hWndFrom, PCOPYDATASTRUCT pstCopyData )
{
	TCHAR	atBuff[MAX_PATH];

	StringCchCopy( atBuff, MAX_PATH, (LPTSTR)(pstCopyData->lpData) );

	TRACE( TEXT("COPYDATA[%s]"), atBuff );

	DocDoOpenFile( hWnd, atBuff );	//	�ʂɋN�������������ł����t�@�C�����J��

	return;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	�h���b�O���h���b�v�̎󂯓���
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	hDrop	�h���b�s���I�u�W�F�N�g�n���h�D
*/
VOID Cls_OnDropFiles( HWND hWnd, HDROP hDrop )
{
	TCHAR	atFileName[MAX_PATH];
//	LPARAM	dNumber;

	ZeroMemory( atFileName, sizeof(atFileName) );

	DragQueryFile( hDrop, 0, atFileName, MAX_PATH );
	DragFinish( hDrop );

	TRACE( TEXT("DROP[%s]"), atFileName );

	DocDoOpenFile( hWnd, atFileName );	//	�c���c�ŊJ��

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�z�b�g�L�[�����ꂽ��Ă΂��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	idHotKey	�o�^���Ă���h�c
	@param[in]	fuModifiers	CTRL�ASHIFT�AALT �̑g�ݍ��킹���
	@param[in]	vk			������Ă����ʃL�[
	@return		����
*/
VOID Cls_OnHotKey(HWND hWnd, INT idHotKey, UINT fuModifiers, UINT vk )
{
	if( VK_D == vk )
	{
		TRACE( TEXT("Hotkey Incoming!!") );
		DocThreadDropCopy(  );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I�[�i�[�h���[�̏���
	@param[in]	hWnd			�E�C���h�E�n���h��
	@param[in]	*pstDrawItem	�I�[�i�h���[�f�[�^
	@return		����
*/
VOID Cls_OnDrawItem( HWND hWnd, CONST DRAWITEMSTRUCT *pstDrawItem )
{
	UINT	dBytes;
	TCHAR	atBuff[SUB_STRING];
	RECT	rect;

	//	�X�e�[�^�X�o�[�ȊO�͊֌W�Ȃ��̂ŕ��u
	if( IDSB_VIEW_STATUS_BAR != pstDrawItem->CtlID )	return;

	if( SB_BYTECNT == pstDrawItem->itemID )
	{
		SetBkMode( pstDrawItem->hDC, TRANSPARENT );
		rect = pstDrawItem->rcItem;

		dBytes =  pstDrawItem->itemData;	//	������|�C���^�Ƃ������Ă�
		StringCchPrintf( atBuff, SUB_STRING, TEXT("%d Bytes"), dBytes );

		if( gdPageByteMax < dBytes )	FillRect( pstDrawItem->hDC, &(pstDrawItem->rcItem), ghStsRedBrush );

		DrawText( pstDrawItem->hDC, atBuff, -1, &(rect), DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�e�[�^�X�o�[�ɕ�����ǉ�
	@param[in]	room	�����g�O�C���f�b�N�X
	@param[in]	ptText	����镶����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MainStatusBarSetText( INT room, LPCTSTR ptText )
{
	StatusBar_SetText( ghStsBarWnd, room , ptText );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�e�[�^�X�o�[�Ƀo�C�g�����I�[�i�[�h���[�œ���
	@param[in]	dByte	�o�C�g��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MainSttBarSetByteCount( UINT dByte )
{
	SendMessage( ghStsBarWnd, SB_SETTEXT, (WPARAM)(SB_BYTECNT | SBT_OWNERDRAW), (LPARAM)dByte );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------




/*!
	���j���[���ڂ��`�F�b�N������O������
	@param[in]	itemID	���삷�郁�j���[�̂h�c
	@param[in]	bCheck	��O�`�F�b�N�E�O�͂���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MenuItemCheckOnOff( UINT itemID, UINT bCheck )
{
	CheckMenuItem( ghMenu, itemID, bCheck ? MF_CHECKED : MF_UNCHECKED );

	ToolBarCheckOnOff( itemID, bCheck );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�O���Ńu���V�@�\ON/OFF�����ꍇ
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT BrushModeToggle( VOID )
{
	LRESULT	rlst;
	HMENU	hSubMenu;

	rlst = SendMessage( ghBrTmplWnd, WMP_BRUSH_TOGGLE, 0, 0 );
	hSubMenu = GetSubMenu( ghMenu, 1 );
	CheckMenuItem( hSubMenu, IDM_BRUSH_STYLE, rlst ? MF_CHECKED : MF_UNCHECKED );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���E�C���h�E�̃N���C�����g�̈�����߂�
	@param[out]	pstRect	�̈悢���\���̃|�C���^�[
*/
UINT AppClientAreaCalc( LPRECT pstRect )
{
	RECT	rect, sbRect, tbRect;
	RECT	ftRect;


	if( !(pstRect) )	return 0;

	if( !(ghMainWnd)  ){	SetRect( pstRect , 0, 0, 0, 0 );	return 0;	}

	GetClientRect( ghMainWnd, &rect );	//	

	ToolBarSizeGet( &tbRect );	//	�c�[���o�[�̃T�C�Y�Ƃ���
	rect.top      += tbRect.bottom;
	rect.bottom   -= tbRect.bottom;

	GetClientRect( ghStsBarWnd, &sbRect );	//	�X�e�[�^�X�o�[�̃T�C�Y�m�F
	rect.bottom   -= sbRect.bottom;

	GetWindowRect( ghFileTabWnd, &ftRect );	//	�^�u�o�[�̃T�C�Y�m�F
	ftRect.bottom -= ftRect.top;
	SetWindowPos( ghFileTabWnd, HWND_TOP, 0, tbRect.bottom, rect.right, ftRect.bottom, SWP_NOZORDER );
	rect.top      += ftRect.bottom;
	rect.bottom   -= ftRect.bottom;	//	�^�u�o�[�̕��k�߂�

	SetRect( pstRect, rect.left, rect.top, rect.right, rect.bottom );

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g���X�̊e���[�h�̐��l
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	pstInfo	���[�h�Z�[�u�̓��e�ۑ�
	@return		��O���[�h�n�j�@�O�Ȃ�����
*/
INT InitTraceValue( UINT dMode, LPTRACEPARAM pstInfo )
{
	TCHAR	atBuff[MIN_STRING];
	INT	iBuff = 0;

	if( dMode )	//	���[�h
	{
		iBuff = GetPrivateProfileInt( TEXT("Trace"), TEXT("Turning"), -1, gatIniPath );
		if( 0 > iBuff )	return 0;

		pstInfo->dTurning     = iBuff;
		pstInfo->dZooming     = GetPrivateProfileInt( TEXT("Trace"), TEXT("Zooming"), 0, gatIniPath );
		pstInfo->dGrayMoph    = GetPrivateProfileInt( TEXT("Trace"), TEXT("GrayMoph"), 0, gatIniPath );
		pstInfo->dGamma       = GetPrivateProfileInt( TEXT("Trace"), TEXT("Gamma"), 0, gatIniPath );
		pstInfo->dContrast    = GetPrivateProfileInt( TEXT("Trace"), TEXT("Contrast"), 0, gatIniPath );
		pstInfo->stOffsetPt.y = GetPrivateProfileInt( TEXT("Trace"), TEXT("OffsetY"), 0, gatIniPath );
		pstInfo->stOffsetPt.x = GetPrivateProfileInt( TEXT("Trace"), TEXT("OffsetX"), 0, gatIniPath );
		pstInfo->bUpset       = GetPrivateProfileInt( TEXT("Trace"), TEXT("Upset"), 0, gatIniPath );
		pstInfo->bMirror      = GetPrivateProfileInt( TEXT("Trace"), TEXT("Mirror"), 0, gatIniPath );
		pstInfo->dMoziColour  = GetPrivateProfileInt( TEXT("Trace"), TEXT("MoziColour"), 0, gatIniPath );
	}
	else	//	�Z�[�u
	{
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->stOffsetPt.x );
		WritePrivateProfileString( TEXT("Trace"), TEXT("OffsetX"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->stOffsetPt.y );
		WritePrivateProfileString( TEXT("Trace"), TEXT("OffsetY"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dContrast );
		WritePrivateProfileString( TEXT("Trace"), TEXT("Contrast"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dGamma );
		WritePrivateProfileString( TEXT("Trace"), TEXT("Gamma"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dGrayMoph );
		WritePrivateProfileString( TEXT("Trace"), TEXT("GrayMoph"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dZooming );
		WritePrivateProfileString( TEXT("Trace"), TEXT("Zooming"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dTurning );
		WritePrivateProfileString( TEXT("Trace"), TEXT("Turning"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), pstInfo->bUpset );
		WritePrivateProfileString( TEXT("Trace"), TEXT("Upset"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), pstInfo->bMirror );
		WritePrivateProfileString( TEXT("Trace"), TEXT("Mirror"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), pstInfo->dMoziColour );
		WritePrivateProfileString( TEXT("Trace"), TEXT("MoziColour"), atBuff, gatIniPath );
	}


	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�F�ݒ�̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dStyle	�p�����[�^�̎��
	@param[in]	nColour	���[�h�F�f�t�H�l�@�Z�[�u�F�l
	@return	COLORREF	���[�h�F�l�@�Z�[�u�F�O
*/
COLORREF InitColourValue( UINT dMode, UINT dStyle, COLORREF nColour )
{
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	LPTSTR	ptEnd;

	switch( dStyle )
	{
		case CLRV_BASICPEN:	StringCchCopy( atKeyName, SUB_STRING, TEXT("BasicPen") );	break;	//	�����F
		case CLRV_BASICBK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("BasicBack") );	break;	//	�w�i�F
		case CLRV_GRIDLINE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("GridLine") );	break;	//	�O���b�h
		case CLRV_CRLFMARK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("CrLfMark") );	break;	//	���s�}�[�N
		case CLRV_CANTSJIS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("CantSjis") );	break;	//	Unicode����
		default:	return nColour;
	}

	StringCchPrintf( atBuff, MIN_STRING, TEXT("%08X"), nColour );

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( TEXT("Colour"), atKeyName, atBuff, atBuff, MIN_STRING, gatIniPath );
		nColour = (COLORREF)_tcstoul( atBuff, &ptEnd, 16 );
	}
	else	//	�Z�[�u
	{
		WritePrivateProfileString( TEXT("Colour"), atKeyName, atBuff, gatIniPath );
	}

	return nColour;
}
//-------------------------------------------------------------------------------------------------

/*!
	���o�[�z�u�̃Z�[�u���[�h
	@param[in]		dMode	��O���[�h�@�O�Z�[�u
	@param[in]		items	�o���h�̐�
	@param[in,out]	pstInfo	�f�[�^���o��������ꂽ�肷��\���̔z��ւ̃|�C���^�[
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT InitToolBarLayout( UINT dMode, INT items, LPREBARLAYOUTINFO pstInfo )
{
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	UINT	dValue;
	INT	i;


	if( dMode )	//	���[�h
	{
		//	���݊m�F
		dValue = GetPrivateProfileInt( TEXT("ReBar"), TEXT("IDX0_ID"), 0, gatIniPath );
		if( 0 == dValue ){	return E_NOTIMPL;	}	//	�h�c�Ȃ̂łO�ɂ͂Ȃ�Ȃ�

		for( i = 0; items > i; i++ )	//	�C���f�b�N�X��
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("IDX%d_ID"), i );
			pstInfo[i].wID = GetPrivateProfileInt( TEXT("ReBar"), atKeyName, pstInfo[i].wID, gatIniPath );

			StringCchPrintf( atKeyName, MIN_STRING, TEXT("IDX%d_CX"), i );
			pstInfo[i].cx = GetPrivateProfileInt( TEXT("ReBar"), atKeyName, pstInfo[i].cx, gatIniPath );

			StringCchPrintf( atKeyName, MIN_STRING, TEXT("IDX%d_STYLE"), i );
			pstInfo[i].fStyle = GetPrivateProfileInt( TEXT("ReBar"), atKeyName, pstInfo[i].fStyle, gatIniPath );
		}
	}
	else	//	�Z�[�u
	{
		WritePrivateProfileSection( TEXT("ReBar"), NULL, gatIniPath );	//	��U�S�폜

		for( i = 0; items > i; i++ )	//	�C���f�b�N�X��
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("IDX%d_ID"), i );
			StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), pstInfo[i].wID );
			WritePrivateProfileString( TEXT("ReBar"), atKeyName, atBuff, gatIniPath );

			StringCchPrintf( atKeyName, MIN_STRING, TEXT("IDX%d_CX"), i );
			StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), pstInfo[i].cx );
			WritePrivateProfileString( TEXT("ReBar"), atKeyName, atBuff, gatIniPath );

			StringCchPrintf( atKeyName, MIN_STRING, TEXT("IDX%d_STYLE"), i );
			StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), pstInfo[i].fStyle );
			WritePrivateProfileString( TEXT("ReBar"), atKeyName, atBuff, gatIniPath );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�p�����[�^�l�̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dStyle	�p�����[�^�̎��
	@param[in]	nValue	���[�h�F�f�t�H�l�@�Z�[�u�F�l
	@return		INT	���[�h�F�l�@�Z�[�u�F�O
*/
INT InitParamValue( UINT dMode, UINT dStyle, INT nValue )
{
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	INT	iBuff = 0;

	switch( dStyle )	//	INIT_LOAD
	{
		case  VL_CLASHCOVER:	StringCchCopy( atKeyName, SUB_STRING, TEXT("ClashCover") );		break;
		case  VL_GROUP_UNDO:	StringCchCopy( atKeyName, SUB_STRING, TEXT("GroupUndo") );		break;
		case  VL_USE_UNICODE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("UseUnicode")  );	break;
		case  VL_LAYER_TRANS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("LayerTrans")  );	break;
		case  VL_RIGHT_SLIDE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("RightSlide")  );	break;
		case  VL_MAA_SPLIT:		StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaSplit") );		break;
		case  VL_MAA_LCLICK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaMethod") );		break;
		case  VL_UNILISTLAST:	StringCchCopy( atKeyName, SUB_STRING, TEXT("UniListLast") );	break;
		case  VL_MAATIP_VIEW:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaToolTip")  );	break;
		case  VL_MAATIP_SIZE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaToolTipSize") );	break;
		case  VL_LINETMP_CLM:	StringCchCopy( atKeyName, SUB_STRING, TEXT("LineTmplClm") );	break;
		case  VL_BRUSHTMP_CLM:	StringCchCopy( atKeyName, SUB_STRING, TEXT("BrushTmplClm") );	break;
		case  VL_UNIRADIX_HEX:	StringCchCopy( atKeyName, SUB_STRING, TEXT("UniRadixHex") );	break;
		case  VL_BACKUP_INTVL:	StringCchCopy( atKeyName, SUB_STRING, TEXT("BackUpIntvl") );	break;
		case  VL_BACKUP_MSGON:	StringCchCopy( atKeyName, SUB_STRING, TEXT("BackUpMsgOn") );	break;
		case  VL_CRLF_CODE:		StringCchCopy( atKeyName, SUB_STRING, TEXT("CrLfCode") );		break;
		case  VL_SPACE_VIEW:	StringCchCopy( atKeyName, SUB_STRING, TEXT("SpaceView") );		break;
		case  VL_GRID_X_POS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("GridXpos") );		break;
		case  VL_GRID_Y_POS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("GridYpos") );		break;
		case  VL_GRID_VIEW:		StringCchCopy( atKeyName, SUB_STRING, TEXT("GridView") );		break;
		case  VL_MAA_TOPMOST:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaViewOn") );		break;
		case  VL_R_RULER_POS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("RightRuler")  );	break;
		case  VL_R_RULER_VIEW:	StringCchCopy( atKeyName, SUB_STRING, TEXT("RitRulerView") );	break;
		case  VL_PAGETIP_VIEW:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageToolTip") );	break;
		case  VL_PCOMBINE_NM:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageCombineNM") );	break;
		case  VL_PDIVIDE_NM:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageDivideNM") );	break;
		case  VL_PDELETE_NM:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageDeleteNM") );	break;
		case  VL_MAASEP_STYLE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaSepLine")  );	break;
		case  VL_PLS_LN_DOCK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PLstLineDock") );	break;
	//	case  VL_BRUSH_DOCK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("BrushDock") );		break;
		case  VL_SWAP_COPY:		StringCchCopy( atKeyName, SUB_STRING, TEXT("CopyModeSwap") );	break;
		case  VL_MAIN_SPLIT:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MainSplit") );		break;
		case  VL_MAXIMISED:		StringCchCopy( atKeyName, SUB_STRING, TEXT("Maximised") );		break;
		case  VL_DRT_LCLICK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("DraughtDefault") );	break;
		case  VL_FIRST_READED:	StringCchCopy( atKeyName, SUB_STRING, TEXT("FirstStep") );		break;
		case  VL_LAST_OPEN:		StringCchCopy( atKeyName, SUB_STRING, TEXT("LastOpenStyle") );	break;
		case  VL_MAA_MCLICK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaSubMethod") );	break;
		case  VL_DRT_MCLICK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("DraughtSubDef") );	break;
		case  VL_WORKLOG:		StringCchCopy( atKeyName, SUB_STRING, TEXT("WorkLogDebug") );	break;
		case  VL_PAGE_UNDER:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageNumUnder") );	break;
		case  VL_PAGE_OVWRITE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageNumOvWrite") );	break;
		case  VL_MAA_RETFCS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaRetFocus") );	break;
		case  VL_PGL_RETFCS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageRetFocus") );	break;
		case  VL_U_RULER_POS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("UnderRuler")  );	break;
		case  VL_U_RULER_VIEW:	StringCchCopy( atKeyName, SUB_STRING, TEXT("UndRulerView") );	break;
		case  VL_PAGEBYTE_MAX:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageByteMax") );	break;
		case  VL_MAA_BKCOLOUR:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaBkColour") );	break;
		case  VL_THUMB_HORIZ:	StringCchCopy( atKeyName, SUB_STRING, TEXT("ThumbHoriz")  );	break;
		case  VL_THUMB_VERTI:	StringCchCopy( atKeyName, SUB_STRING, TEXT("ThumbVerti")  );	break;
		case  VL_MULTI_ACT_E:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MultiAct") );		break;
		case  VL_SAVE_MSGON:	StringCchCopy( atKeyName, SUB_STRING, TEXT("SaveMsgOn") );		break;
		case  VL_MAATAB_SNGL:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaTabSingle") );	break;
		case  VL_HINT_ENABLE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("HintPopup") );		break;
		case  VL_SPMOZI_ENC:	StringCchCopy( atKeyName, SUB_STRING, TEXT("SpMoziEnc") );		break;	//	SPMOZI_ENCODE
		default:	return nValue;
	}

	StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), nValue );

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( TEXT("General"), atKeyName, atBuff, atBuff, MIN_STRING, gatIniPath );
		iBuff = StrToInt( atBuff );
	}
	else	//	�Z�[�u
	{
		WritePrivateProfileString( TEXT("General"), atKeyName, atBuff, gatIniPath );
	}

	return iBuff;
}
//-------------------------------------------------------------------------------------------------

/*!
	������̐ݒ���e���Z�[�u���[�h
	@param[in]		dMode	��O���[�h�@�O�Z�[�u
	@param[in]		dStyle	�p�����[�^�̎��
	@param[in,out]	ptStr	���[�h�F�f�t�H���g������@�Z�[�u�F�ۑ����镶����@MAX_PATH�ł��邱��
	@return			HRESULT	�I����ԃR�[�h
*/
HRESULT InitParamString( UINT dMode, UINT dStyle, LPTSTR ptStr )
{
	TCHAR	atKeyName[MIN_STRING], atDefault[MAX_PATH];

	if( !(ptStr) )	return E_INVALIDARG;

	switch( dStyle )
	{
		case VS_PROFILE_NAME:	StringCchCopy( atKeyName, SUB_STRING, TEXT("ProfilePath") );	break;
		case VS_PAGE_FORMAT:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageFormat")  );	break;
		case VS_FONT_NAME:		StringCchCopy( atKeyName, SUB_STRING, TEXT("FontName") );		break;
		case VS_UNI_USE_LOG:	StringCchCopy( atKeyName, SUB_STRING, TEXT("UniUseLog") );		break;
		case VS_RGUIDE_MOZI:	StringCchCopy( atKeyName, SUB_STRING, TEXT("RightGuideMozi") );	break;
		case VS_EXT_M2H_PATH:	StringCchCopy( atKeyName, SUB_STRING, TEXT("ExtM2HPath") );		break;
		default:	return E_INVALIDARG;
	}

	if( dMode )	//	���[�h
	{
		StringCchCopy( atDefault, MAX_PATH, ptStr );
		GetPrivateProfileString( TEXT("General"), atKeyName, atDefault, ptStr, MAX_PATH, gatIniPath );
	}
	else
	{
		WritePrivateProfileString( TEXT("General"), atKeyName, ptStr, gatIniPath );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�őO�ʏ�Ԃ̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dStyle	�Q�ňꗗ�@�R��s�@�R�u���V�@�S�����s
	@param[in]	nValue	���[�h�F�f�t�H�l�@�Z�[�u�F�l
	@return		INT	���[�h�F�l�@�Z�[�u�F�O
*/
INT InitWindowTopMost( UINT dMode, UINT dStyle, INT nValue )
{
	TCHAR	atAppName[MIN_STRING], atBuff[MIN_STRING];
	INT	iBuff = 0;

	switch( dStyle )
	{
		case  WDP_PLIST:	StringCchCopy( atAppName, SUB_STRING, TEXT("PageList") );	break;
		case  WDP_LNTMPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("LineTmple") );	break;
		case  WDP_BRTMPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("BrushTmple") );	break;
		case  WDP_MAATPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("MaaTmple") );	break;
		default:	return 0;
	}

	StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), nValue );

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( atAppName, TEXT("TopMost"), atBuff, atBuff, MIN_STRING, gatIniPath );
		iBuff = StrToInt( atBuff );
	}
	else	//	�Z�[�u
	{
		WritePrivateProfileString( atAppName, TEXT("TopMost"), atBuff, gatIniPath );
	}

	return iBuff;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�ʒu�̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dStyle	�P�r���[�@�Q�ňꗗ�@�R��s�@�R�u���V�@�S�����s
	@param[in]	pstRect	���[�h���ʂ����邩�A�Z�[�u���e������
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT InitWindowPos( UINT dMode, UINT dStyle, LPRECT pstRect )
{
	TCHAR	atAppName[MIN_STRING], atBuff[MIN_STRING];

	if( !pstRect )	return E_INVALIDARG;

	switch( dStyle )
	{
		case  WDP_MVIEW:	StringCchCopy( atAppName, SUB_STRING, TEXT("MainView") );	break;
		case  WDP_PLIST:	StringCchCopy( atAppName, SUB_STRING, TEXT("PageList") );	break;
		case  WDP_LNTMPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("LineTmple") );	break;
		case  WDP_BRTMPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("BrushTmple") );	break;
		case  WDP_MAATPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("MaaTmple") );	break;
		case  WDP_PREVIEW:	StringCchCopy( atAppName, SUB_STRING, TEXT("Preview") );	break;
		case  WDP_MMAATPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("MnMaaTmple") );	break;
		default:	return E_INVALIDARG;
	}

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( atAppName, TEXT("LEFT"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->left   = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("TOP"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->top    = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("RIGHT"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->right  = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("BOTTOM"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->bottom = StrToInt( atBuff );
	}
	else	//	�Z�[�u
	{
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->left );
		WritePrivateProfileString( atAppName, TEXT("LEFT"), atBuff, gatIniPath );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->top );
		WritePrivateProfileString( atAppName, TEXT("TOP"), atBuff, gatIniPath );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->right );
		WritePrivateProfileString( atAppName, TEXT("RIGHT"), atBuff, gatIniPath );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->bottom );
		WritePrivateProfileString( atAppName, TEXT("BOTTOM"), atBuff, gatIniPath );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#if 0
/*!
	�ŏI�g�p�t�@�C���̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[out]	ptFile	�t���p�X�EMAX_PATH�ł��邱��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT InitLastOpen( UINT dMode, LPTSTR ptFile )
{
	if(  !(ptFile) )	return E_INVALIDARG;
	if( dMode ){	GetPrivateProfileString( TEXT("History"), TEXT("LastOpen"), TEXT(""), ptFile, MAX_PATH, gatIniPath );	}
	else{			WritePrivateProfileString( TEXT("History") , TEXT("LastOpen"), ptFile, gatIniPath );	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	�g���̃Z�[�u���[�h
	@param[in]	nowWnd	���̑��ԍ�
	@param[in]	iDir	�W�����v�����E�{�P���|�P�ŁE�O�͕s��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT WindowFocusChange( INT nowWnd, INT iDir )
{
	INT	nextWnd;

	if( gbTmpltDock )	//	��������������
	{
		if( WND_MAIN == nowWnd )	nextWnd = WND_MAAT;
		else						nextWnd = WND_MAIN;
	}
	else
	{
		nextWnd = nowWnd + iDir;
		if( 0 >= nextWnd )			nextWnd = WND_BRUSH;	//	WND_BRUSH
		if( WND_BRUSH < nextWnd )	nextWnd = WND_MAIN;
		//	���[�ɒ��ӁE�������������Ȃ��Ǝv��
	}

	switch( nextWnd )
	{
		default:
		case WND_MAIN:	SetForegroundWindow( ghMainWnd );	break;
		case WND_MAAT:	SetForegroundWindow( ghMaaWnd );	break;
		case WND_PAGE:	SetForegroundWindow( ghPgVwWnd );	break;
		case WND_LINE:	SetForegroundWindow( ghLnTmplWnd );	break;
		case WND_BRUSH:	SetForegroundWindow( ghBrTmplWnd );	break;
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�v���t������INI����ǂ񂾂菑������
	@param[in]		dMode	��O���[�h�@�O�Z�[�u
	@param[in]		dNumber	���[�h�Z�[�u�ԍ�
	@param[in,out]	ptFile	���[�h�F���g������@�Z�[�u�F�ۑ����镶����@MAX_PATH�ł��邱�ƁENULL�Ȃ���e����
	@return			HRESULT	�I����ԃR�[�h
*/
HRESULT InitProfHistory( UINT dMode, UINT dNumber, LPTSTR ptFile )
{
	TCHAR	atKeyName[MIN_STRING], atDefault[MAX_PATH];

	if( dMode  )	//	���[�h
	{
		ZeroMemory( ptFile, sizeof(TCHAR) * MAX_PATH );

		StringCchPrintf( atKeyName, MIN_STRING, TEXT("Hist%X"), dNumber );
		GetPrivateProfileString( TEXT("ProfHistory"), atKeyName, TEXT(""), atDefault, MAX_PATH, gatIniPath );

		if( NULL == atDefault[0] )	return E_NOTIMPL;	//	�L�^����

		StringCchCopy( ptFile, MAX_PATH, atDefault );
	}
	else	//	�Z�[�u
	{
		if( ptFile )
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Hist%X"), dNumber );
			WritePrivateProfileString( TEXT("ProfHistory"), atKeyName, ptFile, gatIniPath );
		}
		else	//	��U�S�폜
		{
			WritePrivateProfileSection( TEXT("ProfHistory"), NULL, gatIniPath );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�e���̕\���ʒu�Ƒ傫�����f�t�H���g�ɖ߂�
	@param[in]	hWnd	�E�C���h�E�n���h��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT WindowPositionReset( HWND hWnd )
{
	HWND	hWorkWnd;
	RECT	rect;

	TRACE(TEXT("���ʒu��񃊃Z�b�g") );

	//	���C����
	hWorkWnd = GetDesktopWindow( );
	GetWindowRect( hWorkWnd, &rect );
	rect.left   = ( rect.right  - W_WIDTH ) / 3;	//	�����
	rect.top    = ( rect.bottom - W_HEIGHT ) / 2;
	rect.right  = W_WIDTH;
	rect.bottom = W_HEIGHT;
	//	�ʒu�ύX
	SetWindowPos( ghMainWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW );

	//	���C���X�v���b�g�o�[
	if( ghMainSplitWnd )
	{
		AppClientAreaCalc( &rect  );	//	�E�ɕ����Ĉړ�
		SetWindowPos( ghMainSplitWnd, HWND_TOP, rect.right - PLIST_DOCK, rect.top, 0, 0, SWP_NOSIZE );

		ViewSizeMove( hWnd, &rect );	//	�ʒu��񃊃Z�b�g����
	}
	else	//	�t���[�e�B���O�E�C���h�E
	{
		PageListPositionReset( ghMainWnd );
		LineTmplePositionReset( ghMainWnd );
		BrushTmplePositionReset( ghMainWnd );
	}

	MaaTmpltPositionReset( ghMainWnd );	//	�l�`�`��

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C�����痚����荞�񂾂菑�����񂾂�
	@param[in]	hWnd	�E�C���h�E�n���h���ENULL�Ȃ�f�X�g���C
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT OpenHistoryInitialise( HWND hWnd )
{
	HMENU	hSubMenu;
	TCHAR	atKeyName[MIN_STRING], atString[MAX_PATH+10];
	UINT	d;
	UINT_PTR	dItems;
	OPENHIST	stOpenHist;
	OPHIS_ITR	itHist;

	if( hWnd )
	{
		gltOpenHist.clear( );	//	�Ƃ肠�����S�폜

		for( d = 0; OPENHIST_MAX > d; d++ )
		{
			ZeroMemory( &stOpenHist, sizeof(OPENHIST) );
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Hist%X"), d );
			GetPrivateProfileString( TEXT("OpenHistory"), atKeyName, TEXT(""), stOpenHist.atFile, MAX_PATH, gatIniPath );
			if( NULL == stOpenHist.atFile[0] )	break;	//	�L�^�����Ȃ����炻���ŏI������

			gltOpenHist.push_back( stOpenHist );
		}

		if( ghHistyMenu )	DestroyMenu( ghHistyMenu );
		//	���j���[�쐬
		ghHistyMenu = CreatePopupMenu(  );
		AppendMenu( ghHistyMenu, MF_SEPARATOR, 0, NULL );	//	�Z�b�p���^�[
		AppendMenu( ghHistyMenu, MF_STRING, IDM_OPEN_HIS_CLEAR, TEXT("�����N����") );
		//������Œ�͂��܂�C�N�i�C

		dItems = gltOpenHist.size( );
		if( 0 == dItems )
		{
			//	�I�[�|�������������ꍇ
			InsertMenu( ghHistyMenu, 0, MF_STRING | MF_BYPOSITION | MF_GRAYED, IDM_OPEN_HIS_FIRST, TEXT("(��)") );
		}
		else
		{
			//	�I�[�|����������ׂ�
			for( itHist = gltOpenHist.begin(), d = dItems-1; gltOpenHist.end() != itHist; itHist++, d-- )
			{
				StringCchPrintf( atString, MAX_PATH+10, TEXT("(&%X) %s"), d, itHist->atFile );
				InsertMenu( ghHistyMenu, 0, MF_STRING | MF_BYPOSITION, (IDM_OPEN_HIS_FIRST + d), atString );
				itHist->dMenuNumber = (IDM_OPEN_HIS_FIRST + d);
			}
		}

#pragma message ("�t�@�C���I�[�|�������̊�_���j���[�A�ʒu�ɒ���")
		//	���j���[���̏���
		hSubMenu = GetSubMenu( ghMenu, 0 );
		ModifyMenu( hSubMenu, 2, MF_BYPOSITION | MF_POPUP, (UINT_PTR)ghHistyMenu, TEXT("�t�@�C���g�p����(&H)") );
		//������Œ�͂��܂�C�N�i�C

		DrawMenuBar( hWnd );	//	�v��Ȃ������H
	}
	else	//	APPZ�I����
	{
		if( ghHistyMenu )	DestroyMenu( ghHistyMenu );

		WritePrivateProfileSection( TEXT("OpenHistory"), NULL, gatIniPath );	//	��U�S�폜

		//	���g��ۑ�
		for( itHist = gltOpenHist.begin(), d = 0; gltOpenHist.end() != itHist; itHist++, d++ )
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Hist%X"), d );
			WritePrivateProfileString( TEXT("OpenHistory"), atKeyName, itHist->atFile, gatIniPath );
		}

	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J����������ԍ��w�肵�ēǂݍ���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	id		�����w�胁�b�Z�[�W�E���j���[�h�c�ł���
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT OpenHistoryLoad( HWND hWnd, INT id )
{
	UINT_PTR	dNumber, dItems;
	LPARAM		lUnique;
	OPHIS_ITR	itHist;

	dNumber = id - IDM_OPEN_HIS_FIRST;

	TRACE( TEXT("���� -> %d"), dNumber );
	if( OPENHIST_MAX <= dNumber ){	return E_OUTOFMEMORY;	}

	dItems = gltOpenHist.size();
	dNumber = (dItems-1) - dNumber;

	itHist = gltOpenHist.begin();
	std::advance( itHist , dNumber );	//	�����i�߂�

	lUnique = DocOpendFileCheck( itHist->atFile );
	if( 1 <= lUnique )	//	�����̃t�@�C���q�b�g�E�������Ɉړ�����
	{
		if( SUCCEEDED( MultiFileTabSelect( dNumber ) ) )
		{
			DocMultiFileSelect( lUnique );	//	���̃^�u�̃t�@�C����\��
		}
	}
	else
	{
		DocDoOpenFile( hWnd, itHist->atFile );	//	��������I�������t�@�C�����J��
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J�����t�@�C�����L�^����
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[out]	ptFile	�J�����t�@�C���̂��o�b�t�@�EMAX_PATH�ł��邱�ƁENULL�Ȃ痚���N�����[
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT OpenHistoryLogging( HWND hWnd, LPTSTR ptFile )
{
	UINT_PTR	dItems;
	OPENHIST	stOpenHist;
	OPHIS_ITR	itHist;

	if( ptFile )
	{
		ZeroMemory( &stOpenHist, sizeof(OPENHIST) );

		StringCchCopy( stOpenHist.atFile, MAX_PATH, ptFile );
		//�����̓��e�Ȃ�ŐV�ɓ���ւ���̂ŁA�������Ă���
		for( itHist = gltOpenHist.begin(); gltOpenHist.end() != itHist; itHist++ )
		{
			if( !StrCmp( itHist->atFile, stOpenHist.atFile ) )	//	�������̂���������폜����
			{
				gltOpenHist.erase( itHist );
				break;
			}
		}

		gltOpenHist.push_back( stOpenHist );	//	���X�g�����قǐV����

		//	�����͂ݏo���悤�Ȃ�Â��̂��폜����
		dItems = gltOpenHist.size( );
		if( OPENHIST_MAX <  dItems )
		{
			gltOpenHist.pop_front(  );
		}
	}
	else	//	������w�薳���ꍇ�͑S�N��
	{
		gltOpenHist.clear();
	}

	OpenHistoryInitialise( NULL );	//	�Â��̔j�󂵂�
	OpenHistoryInitialise( hWnd );	//	�ŐV�̓��e�ō�蒼��

	return S_OK;
}
//-------------------------------------------------------------------------------------------------



/*!
	���Ղ����_�C�����O�J��
*/
HRESULT OptionDialogueOpen( VOID )
{
	UINT	bURHbuff, bABUIbuff;
	TCHAR	atFontName[LF_FACESIZE];	//	�t�H���g���ύX�ɔ�����
	//	���ڑ�������A�\���̂ɂ����Ⴄ

	ZeroMemory( atFontName, sizeof(atFontName) );

	bURHbuff  = gbUniRadixHex;
	bABUIbuff = gdBUInterval;
	//	���s�R�[�h�ύX�Ȃ�o�C�g���Čv�Z���K�v��
	DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_GENERAL_OPTION_DLG), ghMainWnd, OptionDlgProc, (LPARAM)atFontName );

	//	�ݒ�ύX����Ă�����e����
	if( bURHbuff != gbUniRadixHex ){	UnicodeRadixExchange( NULL );	};
	if( bABUIbuff != gdBUInterval )
	{
		if( 1 <= bABUIbuff )	KillTimer( ghMainWnd, IDT_BACKUP_TIMER );
		if( 1 <= gdBUInterval )	SetTimer(  ghMainWnd, IDT_BACKUP_TIMER, (gdBUInterval * 60000), NULL );
	}

	MenuItemCheckOnOff( IDM_UNICODE_TOGGLE , gbUniPad );	 //	���j�R�[�hON/OFF�̃`�F�L

	SqnSetting(   );	//	�O���[�v�A���h�D�̃A��

	if( NULL != atFontName[ 0] )	//	�t�H���g���A�Ȃ񂩓����Ă���ύX���ꂽ�ƌ��Ȃ�
	{
		InitParamString( INIT_SAVE, VS_FONT_NAME, atFontName );
		//	�L�^����E�����͂��Ƃ�
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�O���c�[��MLT2HTML�̃p�X�ݒ�
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID OptionExtMlt2HtmlPath( HWND hDlg )
{
	OPENFILENAME	stOpenFile;
//	HWND	hEditWnd;
	 BOOLEAN	bOpened;
	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING];


	ZeroMemory( &stOpenFile, sizeof(OPENFILENAME) );

	StringCchCopy( atFilePath, MAX_PATH, TEXT("MLT2HTML.exe") );
	ZeroMemory( atFileName,  sizeof(atFileName) );

	stOpenFile.lStructSize     = sizeof(OPENFILENAME);
	stOpenFile.hwndOwner       = hDlg;
	stOpenFile.lpstrFilter     = TEXT("���s�t�@�C��\0*.exe\0\0");
	stOpenFile.lpstrFile       = atFilePath;
	stOpenFile.nMaxFile        = MAX_PATH;
	stOpenFile.lpstrFileTitle  = atFileName;
	stOpenFile.nMaxFileTitle   = MAX_STRING;
	stOpenFile.lpstrTitle      = TEXT("���s�t�@�C�����w�肵�Ă�����");
	stOpenFile.Flags           = OFN_EXPLORER | OFN_HIDEREADONLY;
	stOpenFile.lpstrDefExt     = TEXT("exe");

	//������ FileOpenDialogue ���o��
	bOpened = GetOpenFileName( &stOpenFile );

	if( !(bOpened) ){	 return;	}	//	�L�����Z�����Ă��牽�����Ȃ�

	SetDlgItemText( hDlg, IDE_EXTERNAL_M2H_PATH, atFilePath );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	���Ղ����_�C�����O�̃v���V�[�W��
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK OptionDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static  UINT	cdGrXp, cdGrYp, cdRtRr, cdUdRr;

	UINT	id;
	INT		dValue, iBuff;
	TCHAR	atBuff[SUB_STRING];
	TCHAR	atPath[MAX_PATH];

	static LPTSTR	ptFontName;
	CHOOSEFONT	stChooseFont;
	LOGFONT		stLogFont;

	switch( message )
	{
		case WM_INITDIALOG:
			ptFontName = (LPTSTR)lParam;	//	�t�H���g���������

			//	�X���C�_�����l�E�������قǔ����Ȃ�
			SendDlgItemMessage( hDlg, IDSL_LAYERBOX_TRANCED, TBM_SETRANGE, TRUE, MAKELPARAM( 0, 0xE0 ) );	//	0xE0

			//	�A�C�e���폜
			//Edit_SetText( GetDlgItem(hDlg,IDE_AA_DIRECTORY), TEXT("�`�`�f�B���N�g���̓v���t�@�C������ݒ肵�Ă�") );
			//EnableWindow( GetDlgItem(hDlg,IDE_AA_DIRECTORY), FALSE );
			//ShowWindow( GetDlgItem(hDlg,IDB_AADIR_SEARCH), SW_HIDE );

			//	MAA�|�b�v�A�b�v�ɂ���
			dValue = InitParamValue( INIT_LOAD, VL_MAATIP_SIZE, 16 );	//	�T�C�Y�m�F
			if( FONTSZ_REDUCE == dValue )	CheckRadioButton( hDlg, IDRB_POPUP_NOMAL, IDRB_POPUP_REDUCE, IDRB_POPUP_REDUCE );
			else							CheckRadioButton( hDlg, IDRB_POPUP_NOMAL, IDRB_POPUP_REDUCE, IDRB_POPUP_NOMAL );
			dValue = InitParamValue( INIT_LOAD, VL_MAATIP_VIEW, 1 );	//	�|�b�p�b�v���邩
			CheckDlgButton( hDlg, IDCB_POPUP_VISIBLE, dValue ? BST_CHECKED : BST_UNCHECKED );

			//	�E�񂹃h�b�g�ʒu
			dValue = InitParamValue( INIT_LOAD, VL_RIGHT_SLIDE, 790 );
			StringCchPrintf( atBuff, SUB_STRING, TEXT("%d"), dValue );
			Edit_SetText( GetDlgItem(hDlg,IDE_RIGHTSLIDE_DOT), atBuff );

			//	�O���b�h�ʒu
			cdGrXp = gdGridXpos;
			StringCchPrintf( atBuff, SUB_STRING, TEXT("%d"), gdGridXpos );
			Edit_SetText( GetDlgItem(hDlg,IDE_GRID_X_POS), atBuff );

			cdGrYp = gdGridYpos;
			StringCchPrintf( atBuff, SUB_STRING, TEXT("%d"), gdGridYpos );
			Edit_SetText( GetDlgItem(hDlg,IDE_GRID_Y_POS), atBuff );

			//	�E���[���ʒu�h�b�g
			cdRtRr = gdRightRuler;
			StringCchPrintf( atBuff, SUB_STRING, TEXT("%d"), gdRightRuler );
			Edit_SetText( GetDlgItem(hDlg,IDE_RIGHT_RULER_POS), atBuff );

			//	�����[���ʒu�s��
			cdUdRr = gdUnderRuler;
			StringCchPrintf( atBuff, SUB_STRING, TEXT("%d"), gdUnderRuler );
			Edit_SetText( GetDlgItem(hDlg,IDE_UNDER_RULER_POS), atBuff );

			//	�ōő�o�C�g��
			StringCchPrintf( atBuff, SUB_STRING, TEXT("%d"), gdPageByteMax );
			Edit_SetText( GetDlgItem(hDlg,IDE_PAGE_BYTE_MAX), atBuff );

			//	�����ۑ��Ԋu
			dValue = InitParamValue( INIT_LOAD, VL_BACKUP_INTVL, 3 );
			StringCchPrintf( atBuff, SUB_STRING, TEXT("%d"), dValue );
			Edit_SetText( GetDlgItem(hDlg,IDE_AUTO_BU_INTVL), atBuff );

			//	�o�b�N�A�b�v���b�Z�[�W
			CheckDlgButton( hDlg, IDCB_AUTOBU_MSG_ON, gbAutoBUmsg ? BST_CHECKED : BST_UNCHECKED );

			//	�ۑ����b�Z�[�W
			CheckDlgButton( hDlg, IDCB_SAVE_MSG_ON, gbSaveMsgOn ? BST_CHECKED : BST_UNCHECKED );

			//	���s�R�[�h�I��
			CheckRadioButton( hDlg, IDRB_CRLF_STRB, IDRB_CRLF_2CH_YY, gbCrLfCode ? IDRB_CRLF_2CH_YY : IDRB_CRLF_STRB );

			//	�󔒃��j�R�[�h�p�f�B���O
			CheckDlgButton( hDlg, IDCB_USE_UNISPACE_SET, gbUniPad ? BST_CHECKED : BST_UNCHECKED );

			//	���j�R�[�h�P�U�i��
			CheckDlgButton( hDlg, IDCB_UNIRADIX_HEX, gbUniRadixHex ? BST_CHECKED : BST_UNCHECKED );

			//	�O���[�v�A���h�D
			dValue = InitParamValue( INIT_LOAD, VL_GROUP_UNDO, 1 );
			CheckDlgButton( hDlg, IDCB_GROUPUNDO_SET, dValue ? BST_CHECKED : BST_UNCHECKED );

			//	���d�N���L��
			dValue = InitParamValue( INIT_LOAD, VL_MULTI_ACT_E, 0 );
			CheckDlgButton( hDlg, IDCB_MULTIACT_ENA, dValue ? BST_CHECKED : BST_UNCHECKED );

			//	�A�C�e���폜
			//EnableWindow( GetDlgItem(hDlg,IDCB_COPY_STYLE_SWAP), FALSE );

			//	�h�b�L���O�X�^�C��
			dValue = InitParamValue( INIT_LOAD, VL_PLS_LN_DOCK, 1 );
			CheckDlgButton( hDlg, IDCB_DOCKING_STYLE, dValue ? BST_CHECKED : BST_UNCHECKED );

			//	�N�����I�[�|��
			dValue = InitParamValue( INIT_LOAD, VL_LAST_OPEN, LASTOPEN_DO );
			CheckRadioButton( hDlg, IDRB_LASTOPEN_DO, IDRB_LASTOPEN_ASK, (IDRB_LASTOPEN_DO + dValue) );

#ifdef TODAY_HINT_STYLE
			//	�����̃q���g
			dValue = InitParamValue( INIT_LOAD, VL_HINT_ENABLE, 1 );
			CheckDlgButton( hDlg, IDCB_POPHINT_VIEW, dValue ? BST_CHECKED : BST_UNCHECKED );
#endif
#ifdef SPMOZI_ENCODE
			//	�@��ˑ������𐔒l�Q�ƃR�s�[
			dValue = InitParamValue( INIT_LOAD, VL_SPMOZI_ENC, 0 );
			CheckDlgButton( hDlg, IDCB_SPMOZI_ENCODE, dValue ? BST_CHECKED : BST_UNCHECKED );
#endif
			//	���C���{�b�N�X�̓����x
			dValue = InitParamValue( INIT_LOAD, VL_LAYER_TRANS, 192 );
			SendDlgItemMessage( hDlg, IDSL_LAYERBOX_TRANCED, TBM_SETPOS, TRUE, (dValue - 0x1F) );

			//	�����s�e���v�����N���b�N�����Ƃ��̓���
			dValue = InitParamValue( INIT_LOAD, VL_MAA_LCLICK, MAA_INSERT );
			switch( dValue )
			{
				default:
				case MAA_INSERT:	id =  IDRB_SEL_INS_EDIT;	break;
				case MAA_INTERRUPT:	id =  IDRB_SEL_INTRPT_EDIT;	break;
				case MAA_LAYERED:	id =  IDRB_SEL_SET_LAYER;	break;
				case MAA_UNICLIP:	id =  IDRB_SEL_CLIP_UNI;	break;
				case MAA_SJISCLIP:	id =  IDRB_SEL_CLIP_SJIS;	break;
				case MAA_DRAUGHT:	id =  IDRB_SEL_DRAUGHT;		break;
			}
			CheckRadioButton( hDlg, IDRB_SEL_INS_EDIT, IDRB_SEL_DRAUGHT, id );

			//	�����s�e���v�����~�b�h�N���b�N�����Ƃ��̓���
			dValue = InitParamValue( INIT_LOAD, VL_MAA_MCLICK, MAA_INSERT );
			switch( dValue )
			{
				default:
				case MAA_INSERT:	id =  IDRB_SELSUB_INS_EDIT;		break;
				case MAA_INTERRUPT:	id =  IDRB_SELSUB_INTRPT_EDIT;	break;
				case MAA_LAYERED:	id =  IDRB_SELSUB_SET_LAYER;	break;
				case MAA_UNICLIP:	id =  IDRB_SELSUB_CLIP_UNI;		break;
				case MAA_SJISCLIP:	id =  IDRB_SELSUB_CLIP_SJIS;	break;
				case MAA_DRAUGHT:	id =  IDRB_SELSUB_DRAUGHT;		break;
			}
			CheckRadioButton( hDlg, IDRB_SELSUB_INS_EDIT, IDRB_SELSUB_DRAUGHT, id );

			//	�h���t�g�{�[�h�ŃN���b�N�����Ƃ��̓���
			switch( gdClickDrt )
			{
				case MAA_INSERT:	id =  IDRB_DRT_INS_EDIT;	break;
				case MAA_INTERRUPT:	id =  IDRB_DRT_INTRPT_EDIT;	break;
				case MAA_LAYERED:	id =  IDRB_DRT_SET_LAYER;	break;
				case MAA_UNICLIP:	id =  IDRB_DRT_CLIP_UNI;	break;
				case MAA_SJISCLIP:	id =  IDRB_DRT_CLIP_SJIS;	break;
			}
			CheckRadioButton( hDlg, IDRB_DRT_INS_EDIT, IDRB_DRT_CLIP_SJIS, id );

			switch( gdSubClickDrt )
			{
				case MAA_INSERT:	id =  IDRB_DRTSUB_INS_EDIT;		break;
				case MAA_INTERRUPT:	id =  IDRB_DRTSUB_INTRPT_EDIT;	break;
				case MAA_LAYERED:	id =  IDRB_DRTSUB_SET_LAYER;	break;
				case MAA_UNICLIP:	id =  IDRB_DRTSUB_CLIP_UNI;		break;
				case MAA_SJISCLIP:	id =  IDRB_DRTSUB_CLIP_SJIS;	break;
			}
			CheckRadioButton( hDlg, IDRB_DRTSUB_INS_EDIT, IDRB_DRTSUB_CLIP_SJIS, id );

			//	MLT2HTML�̃p�X
			ZeroMemory( atPath, sizeof(atPath) );
			InitParamString( INIT_LOAD, VS_EXT_M2H_PATH, atPath );
			SetDlgItemText( hDlg, IDE_EXTERNAL_M2H_PATH, atPath );

			return (INT_PTR)TRUE;

		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
				case IDB_CHOOSEFONT:	//	�t�H���g�ύX�̃A��
					ViewingFontGet( &stLogFont );
					ZeroMemory( &stChooseFont, sizeof(CHOOSEFONT) );
					stChooseFont.lStructSize = sizeof(CHOOSEFONT);
					stChooseFont.hwndOwner = hDlg;
					stChooseFont.lpLogFont = &stLogFont;
					stChooseFont.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
					if( ChooseFont(&stChooseFont) )	//	�m�肵���̂Ȃ璆�g�R�s���Ă���
					{
						StringCchCopy( ptFontName, LF_FACESIZE, stLogFont.lfFaceName );
					}
					break;

				case IDB_EXTERNAL_M2H_SEL:	//	�O���c�[���̃p�X�ݒ�̃A��
					OptionExtMlt2HtmlPath( hDlg );
					break;

				case IDB_APPLY://�K�p
				case IDOK:
					//	�c�[���`�b�v�̕����T�C�Y
					dValue = FONTSZ_NORMAL;
					if( IsDlgButtonChecked( hDlg, IDRB_POPUP_REDUCE ) ){	dValue =  FONTSZ_REDUCE;	}
					InitParamValue( INIT_SAVE, VL_MAATIP_SIZE, dValue );
#ifdef USE_HOVERTIP
					HoverTipSizeChange( dValue );	//	�|�b�p�b�v�S��
#endif
					//	�l�`�`�ŕ\�����邩�ǂ����E����Ȃ��H
					iBuff = IsDlgButtonChecked( hDlg, IDCB_POPUP_VISIBLE );
					AaItemsTipSizeChange( dValue, iBuff );
					InitParamValue( INIT_SAVE, VL_MAATIP_VIEW, iBuff );

					//	�E�񂹊�h�b�g
					Edit_GetText( GetDlgItem(hDlg,IDE_RIGHTSLIDE_DOT), atBuff, SUB_STRING );
					dValue = StrToInt( atBuff );
					InitParamValue( INIT_SAVE, VL_RIGHT_SLIDE, dValue );

					//	�O���b�h�ʒu
					Edit_GetText( GetDlgItem(hDlg,IDE_GRID_X_POS), atBuff, SUB_STRING );
					gdGridXpos = StrToInt( atBuff );
					InitParamValue( INIT_SAVE, VL_GRID_X_POS, gdGridXpos );
					if( cdGrXp != gdGridXpos )	ViewRedrawSetLine( -1 );

					Edit_GetText( GetDlgItem(hDlg,IDE_GRID_Y_POS), atBuff, SUB_STRING );
					gdGridYpos = StrToInt( atBuff );
					InitParamValue( INIT_SAVE, VL_GRID_Y_POS, gdGridYpos );
					if( cdGrYp != gdGridYpos )	ViewRedrawSetLine( -1 );

					//	�E���[���ʒu
					Edit_GetText( GetDlgItem(hDlg,IDE_RIGHT_RULER_POS), atBuff, SUB_STRING );
					gdRightRuler = StrToInt( atBuff );
					InitParamValue( INIT_SAVE, VL_R_RULER_POS, gdRightRuler );
					if( cdRtRr != gdRightRuler )	ViewRedrawSetLine( -1 );

					//	�����[���ʒu�s��
					Edit_GetText( GetDlgItem(hDlg,IDE_UNDER_RULER_POS), atBuff, SUB_STRING );
					gdUnderRuler = StrToInt( atBuff );
					InitParamValue( INIT_SAVE, VL_U_RULER_POS, gdUnderRuler );
					if( cdUdRr != gdUnderRuler )	ViewRedrawSetLine( -1 );

					//	�ōő�o�C�g��
					Edit_GetText( GetDlgItem(hDlg,IDE_PAGE_BYTE_MAX), atBuff, SUB_STRING );
					gdPageByteMax = StrToInt( atBuff );
					InitParamValue( INIT_SAVE, VL_PAGEBYTE_MAX, gdPageByteMax );


					//	�����ۑ��Ԋu
					Edit_GetText( GetDlgItem(hDlg,IDE_AUTO_BU_INTVL), atBuff, SUB_STRING );
					gdBUInterval = StrToInt( atBuff );
					InitParamValue( INIT_SAVE, VL_BACKUP_INTVL, gdBUInterval );
					//	�o�b�N�A�b�v���b�Z�[�W
					dValue = IsDlgButtonChecked( hDlg, IDCB_AUTOBU_MSG_ON );
					gbAutoBUmsg = dValue ? 1 : 0;
					InitParamValue( INIT_SAVE, VL_BACKUP_MSGON, gbAutoBUmsg );

					//	�ۑ����b�Z�[�W
					dValue = IsDlgButtonChecked( hDlg, IDCB_SAVE_MSG_ON );
					gbSaveMsgOn = dValue ? 1 : 0;
					InitParamValue( INIT_SAVE, VL_SAVE_MSGON, gbSaveMsgOn );

					//	���s�R�[�h�I��
					gbCrLfCode = 0;
					if( IsDlgButtonChecked( hDlg , IDRB_CRLF_2CH_YY ) ){	gbCrLfCode =  1;	}
					InitParamValue( INIT_SAVE, VL_CRLF_CODE, gbCrLfCode );

					//	���j�R�[�h�X�y�[�X
					dValue = IsDlgButtonChecked( hDlg, IDCB_USE_UNISPACE_SET );
					gbUniPad = dValue ? 1 : 0;
					InitParamValue( INIT_SAVE, VL_USE_UNICODE, gbUniPad );

					//	���j�R�[�h�P�U�i��
					dValue = IsDlgButtonChecked( hDlg, IDCB_UNIRADIX_HEX );
					gbUniRadixHex = dValue ? 1 : 0;
					InitParamValue( INIT_SAVE, VL_UNIRADIX_HEX, gbUniRadixHex );

					//	�O���[�v�A���h�D
					dValue = IsDlgButtonChecked( hDlg, IDCB_GROUPUNDO_SET );
					InitParamValue( INIT_SAVE, VL_GROUP_UNDO, dValue ? 1 : 0 );

					//	20130116	���d�N���L��
					dValue = IsDlgButtonChecked( hDlg, IDCB_MULTIACT_ENA );
					InitParamValue( INIT_SAVE, VL_MULTI_ACT_E, dValue ? 1 : 0 );

					//	�h�b�L���O�X�^�C���E�ύX���Ă��A�ċN������܂ł͖���
					dValue = IsDlgButtonChecked( hDlg, IDCB_DOCKING_STYLE );
					InitParamValue( INIT_SAVE, VL_PLS_LN_DOCK, dValue ? 1 : 0 );

#ifdef TODAY_HINT_STYLE
					//	�����̃q���g
					dValue = IsDlgButtonChecked( hDlg, IDCB_POPHINT_VIEW );
					InitParamValue( INIT_SAVE, VL_HINT_ENABLE, dValue );
#endif
#ifdef SPMOZI_ENCODE
					//	�@��ˑ������𐔒l�Q�ƃR�s�[����
					dValue = IsDlgButtonChecked( hDlg, IDCB_SPMOZI_ENCODE );
					gbSpMoziEnc = dValue ? 1 : 0;
					InitParamValue( INIT_SAVE, VL_SPMOZI_ENC, gbSpMoziEnc );
#endif
					//	�N�����I�[�|��
					if( IsDlgButtonChecked( hDlg, IDRB_LASTOPEN_NON ) ){	dValue = LASTOPEN_NON;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_LASTOPEN_ASK ) ){	dValue = LASTOPEN_ASK;	}
					else{	dValue = LASTOPEN_DO;	}	//	IDRB_LASTOPEN_DO
					InitParamValue( INIT_SAVE, VL_LAST_OPEN, dValue );

					//	�����x
					dValue = SendDlgItemMessage( hDlg, IDSL_LAYERBOX_TRANCED, TBM_GETPOS, 0, 0 );
					dValue += 0x1F;
					InitParamValue( INIT_SAVE, VL_LAYER_TRANS, dValue );
					LayerBoxAlphaSet( dValue );

					//	MAA�̑���
					if( IsDlgButtonChecked( hDlg, IDRB_SEL_INTRPT_EDIT )  ){	dValue = MAA_INTERRUPT;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SEL_SET_LAYER ) ){	dValue = MAA_LAYERED;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SEL_CLIP_UNI ) ){	dValue = MAA_UNICLIP;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SEL_CLIP_SJIS ) ){	dValue = MAA_SJISCLIP;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SEL_DRAUGHT ) ){	dValue = MAA_DRAUGHT;	}
					else{	dValue = MAA_INSERT;	}	//	IDRB_SEL_INS_EDIT
					InitParamValue( INIT_SAVE, VL_MAA_LCLICK, dValue );

					if( IsDlgButtonChecked( hDlg, IDRB_SELSUB_INTRPT_EDIT )   ){	iBuff = MAA_INTERRUPT;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SELSUB_SET_LAYER ) ){	iBuff = MAA_LAYERED;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SELSUB_CLIP_UNI ) ){	iBuff = MAA_UNICLIP;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SELSUB_CLIP_SJIS ) ){	iBuff = MAA_SJISCLIP;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SELSUB_DRAUGHT )  ){	iBuff = MAA_DRAUGHT;	}
					else{	iBuff =  MAA_INSERT;	}	//	IDRB_SEL_INS_EDIT
					InitParamValue( INIT_SAVE, VL_MAA_MCLICK, iBuff );

					ViewMaaItemsModeSet( dValue, iBuff );	//	MAA�ɂ��ݒ肨����

					//	�h���t�g�{�[�h�̑���
					if( IsDlgButtonChecked( hDlg, IDRB_DRT_INTRPT_EDIT )  ){	gdClickDrt = MAA_INTERRUPT;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_DRT_SET_LAYER ) ){	gdClickDrt = MAA_LAYERED;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_DRT_CLIP_UNI ) ){	gdClickDrt = MAA_UNICLIP;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_DRT_CLIP_SJIS ) ){	gdClickDrt = MAA_SJISCLIP;	}
					else{	gdClickDrt = MAA_INSERT;	}	//	IDRB_DRT_INS_EDIT
					InitParamValue( INIT_SAVE, VL_DRT_LCLICK, gdClickDrt );

					if( IsDlgButtonChecked( hDlg, IDRB_DRTSUB_INTRPT_EDIT )   ){	gdSubClickDrt = MAA_INTERRUPT;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_DRTSUB_SET_LAYER ) ){	gdSubClickDrt = MAA_LAYERED;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_DRTSUB_CLIP_UNI ) ){	gdSubClickDrt = MAA_UNICLIP;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_DRTSUB_CLIP_SJIS ) ){	gdSubClickDrt = MAA_SJISCLIP;	}
					else{	gdSubClickDrt = MAA_INSERT;	}	//	IDRB_DRTSUB_INS_EDIT
					InitParamValue( INIT_SAVE, VL_DRT_MCLICK, gdSubClickDrt );

					//	MLT2HTML�̃p�X
					GetDlgItemText( hDlg, IDE_EXTERNAL_M2H_PATH, atPath, MAX_PATH );
					InitParamString( INIT_SAVE, VS_EXT_M2H_PATH, atPath );

					//	�n�j�Ȃ�����Ⴄ
					if( IDOK == id ){	EndDialog( hDlg, IDOK );	}
					return (INT_PTR)TRUE;

				case IDCANCEL:
					EndDialog( hDlg, IDCANCEL );
					return (INT_PTR)TRUE;

				default:	break;
			}
			break;

		default:	break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	���j�R�[�h�󔒂̎g�p�E�s�g�p
*/
UINT UnicodeUseToggle( LPVOID pVoid )
{
	gbUniPad =  !(gbUniPad);	//	�g�O��
	InitParamValue( INIT_SAVE, VL_USE_UNICODE, gbUniPad );
	MenuItemCheckOnOff( IDM_UNICODE_TOGGLE, gbUniPad );

	return gbUniPad;
}
//-------------------------------------------------------------------------------------------------

/*!
	�f�B���N�g���I���_�C�A���O�̕\��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	ptSelDir	�f�B���N�g����������o�b�t�@�ւ̃|�C���^�[
	@param[in]	cchLen		�o�b�t�@�̕������B�o�C�g������Ȃ���
	@return		��O�F�f�B���N�g���Ƃ����@�O�F�L�����Z������
*/
BOOLEAN SelectDirectoryDlg( HWND hWnd, LPTSTR ptSelDir, UINT_PTR cchLen )
{
	BROWSEINFO		stBrowseInfo;
	LPITEMIDLIST	pstItemIDList;
	TCHAR	atDisplayName[MAX_PATH];

	if( !(ptSelDir) )	return FALSE;
	ZeroMemory( ptSelDir, sizeof(TCHAR) * cchLen );

	//	BROWSEINFO�\���̂ɒl��ݒ�
	stBrowseInfo.hwndOwner		 = hWnd;	//	�_�C�A���O�̐e�E�C���h�E�̃n���h��
	stBrowseInfo.pidlRoot		 = NULL;	//	���[�g�f�B���N�g��������ITEMIDLIST�̃|�C���^�ENULL�̏ꍇ�f�X�N�g�b�v
	stBrowseInfo.pszDisplayName	 = atDisplayName;	//	�I�����ꂽ�f�B���N�g�������󂯎��o�b�t�@�̃|�C���^
	stBrowseInfo.lpszTitle		 = TEXT("�`�`�̓����Ă�f�B���N�g����I������́[�I");	//	�c���[�r���[�̏㕔�ɕ\������镶����
	stBrowseInfo.ulFlags		 = BIF_RETURNONLYFSDIRS;	//	�\�������f�B���N�g���̎�ނ������t���O
	stBrowseInfo.lpfn			 = NULL;		//	BrowseCallbackProc�֐��̃|�C���^
	stBrowseInfo.lParam			 = (LPARAM)0;	//	�R�[���o�b�N�֐��ɓn���l

	//	�f�B���N�g���I���_�C�A���O��\��
	pstItemIDList = SHBrowseForFolder( &stBrowseInfo );
	if( !(pstItemIDList) )
	{
		//	�߂�l��NULL�̏ꍇ�A�f�B���N�g�����I�����ꂸ�Ƀ_�C�A���O������ꂽ�Ƃ�������
		return FALSE;
	}
	else
	{
		//	ItemIDList���p�X���ɕϊ�
		if( !SHGetPathFromIDList( pstItemIDList, atDisplayName ) )
		{
			//	�G���[����
			return FALSE;
		}
		//	atDisplayName�ɑI�����ꂽ�f�B���N�g���̃p�X�������Ă�
		StringCchCopy( ptSelDir, cchLen, atDisplayName );

		//	pstItemIDList���J�������߂�
		CoTaskMemFree( pstItemIDList );
	}

	return TRUE;
}
//-------------------------------------------------------------------------------------------------


/*!
	�����t�@�C���^�u�̃T�u�N���X�v���V�[�W��
	@param[in]	hWnd	���X�g�̃n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@return	�������ʂƂ�
*/
LRESULT	CALLBACK gpfFileTabProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		HANDLE_MSG( hWnd, WM_MBUTTONUP, Ftb_OnMButtonUp );

		default:	break;
	}

	return CallWindowProc( gpfOriginFileTabProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	�����t�@�C���^�u�Ń}�E�X�̒��{�^�����������ꂽ�Ƃ�
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			���������N���C�����g�w���W�l
	@param[in]	y			���������N���C�����g�x���W�l
	@param[in]	keyFlags	���ɉ�����Ă�L�[�ɂ���
*/
VOID Ftb_OnMButtonUp( HWND hWnd, INT x, INT y, UINT flags )
{
	INT	curSel;
	TCHITTESTINFO	stTcHitInfo;

	stTcHitInfo.pt.x = x;
	stTcHitInfo.pt.y = y;
	curSel = TabCtrl_HitTest( ghFileTabWnd, &stTcHitInfo );

	TRACE( TEXT("FTAB start TAB [%d] [%d x %d]"), curSel, x, y );

	MultiFileTabClose( curSel );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�N�����̍ŏ��̈�̃t�@�C���^�u��ǉ�
	@param[in]	ptName	�t�@�C����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MultiFileTabFirst( LPTSTR ptName )
{
	TCHAR	atName[MAX_PATH];
	TCITEM		stTcItem;

	StringCchCopy( atName, MAX_PATH, ptName );
	PathStripPath( atName );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask = TCIF_TEXT | TCIF_PARAM;
	stTcItem.pszText = atName;
	stTcItem.lParam  = 1;	//	���j�[�N�ԍ��E���INCREMENT
	TabCtrl_InsertItem( ghFileTabWnd, 1, &stTcItem );

	TabCtrl_DeleteItem( ghFileTabWnd, 0 );

	TabCtrl_SetCurSel( ghFileTabWnd, 0 );

#ifdef BIG_TEXT_SEPARATE
	if( gbSeparateMod )	//	�����Ǎ������̂ŕύX�}�[�N��t����
	{
		DocModifyContent( 1 );
		gbSeparateMod = FALSE;
	}
	//	�^�u���ł��������Ă��珈������K�v������
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	��ڈȍ~�̃t�@�C����ǉ�
	@param[in]	dNumber	�ʂ��ԍ�
	@param[in]	ptName	�t�@�C����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MultiFileTabAppend( LPARAM dNumber, LPTSTR ptName )
{
	INT		iCount;
	TCHAR	atName[MAX_PATH];
	TCITEM		stTcItem;

	StringCchCopy( atName, MAX_PATH, ptName );
	PathStripPath( atName );

	iCount = TabCtrl_GetItemCount( ghFileTabWnd );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask = TCIF_TEXT | TCIF_PARAM;
	stTcItem.pszText = atName;
	stTcItem.lParam  = dNumber;	//	���j�[�N�ԍ�
	TabCtrl_InsertItem( ghFileTabWnd, iCount, &stTcItem );

	TabCtrl_SetCurSel( ghFileTabWnd, iCount );

//	�����Ńt�@�C���I�[�|�����L�^����ƁA�֌W�����̂܂Ŗ����ʂɋL�^���Ă܂�

#ifdef BIG_TEXT_SEPARATE
	if( gbSeparateMod )	//	�����Ǎ������̂ŕύX�}�[�N��t����
	{
		DocModifyContent( 1 );
		gbSeparateMod = FALSE;
	}
	//	�^�u���ł��������Ă��珈������K�v������
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��̒ʂ��ԍ����������^�u���������ă^�u�C���f�b�N�X��Ԃ�
	@param[in]	dNumber	�ʂ��ԍ�
	@return		INT		�^�u�C���f�b�N�X�E�Y���Ȃ�������|�P
*/
INT MultiFileTabSearch( LPARAM dNumber )
{
	INT		iCount, i;
	TCITEM	stTcItem;

	iCount = TabCtrl_GetItemCount( ghFileTabWnd );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask = TCIF_PARAM;

	for( i = 0; iCount > i; i++ )
	{
		TabCtrl_GetItem( ghFileTabWnd, i, &stTcItem );

		if( dNumber == stTcItem.lParam )	return i;
	}

	return -1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��̒ʂ��ԍ����������^�u��I����Ԃɂ���
	@param[in]	dNumber	�ʂ��ԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MultiFileTabSelect( LPARAM dNumber )
{
	INT	iRslt;

	iRslt = MultiFileTabSearch( dNumber );	//	�w��̒ʂ��ԍ����������^�u��������

	if( 0 <= iRslt )
	{
		TabCtrl_SetCurSel( ghFileTabWnd, iRslt );	//	�Y���̃^�u�Ƀt�H�[�J�X�ڂ���
		return S_OK;
	}

	return E_OUTOFMEMORY;
}
//-------------------------------------------------------------------------------------------------

/*!
	�L�[�{�[�h����̃^�u�ړ�����E�ׂ̃^�u�Ɉړ�
	@param[in]	xDir	�����F�E�ց@�����F���ց@�O�Ȃɂ����Ȃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MultiFileTabSlide( INT xDir )
{
	INT		curSel, iCount, iTarget;
	LPARAM	dSele;
	TCITEM	stTcItem;

	if( 0 == xDir ){	return  S_FALSE;	}

	//	�^�u�̑���
	iCount = TabCtrl_GetItemCount( ghFileTabWnd );

	//	�I�����Ă�z��I������
	curSel = TabCtrl_GetCurSel( ghFileTabWnd );

	//	�ړ���^�u������
	if( 0 < xDir )
	{
		iTarget = curSel + 1;
		if( iCount <= iTarget ){	iTarget = 0;	}
	}
	else
	{
		iTarget = curSel - 1;
		if( 0 > iTarget ){	iTarget = iCount - 1;	}
	}

	//	���̃^�u��I����Ԃɂ���
	TabCtrl_SetCurSel( ghFileTabWnd, iTarget );

	//	���̃^�u�̃t�@�C���ԍ����m�F����
	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask = TCIF_PARAM;
	TabCtrl_GetItem( ghFileTabWnd, iTarget, &stTcItem );
	dSele = stTcItem.lParam;

	//	�Y���̃t�@�C�����t�H�[�J�X����
	DocMultiFileSelect( dSele );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��̒ʂ��ԍ����������^�u�̖��̂�ύX
	@param[in]	dNumber	�t�@�C����UNIQUE�ԍ�
	@param[in]	ptName	�t�@�C����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MultiFileTabRename( LPARAM dNumber, LPTSTR ptName )
{
	INT	iRslt;
	TCHAR	atName[MAX_PATH];
	TCITEM		stTcItem;

	iRslt = MultiFileTabSearch( dNumber );
	if( 0 > iRslt ){	return E_OUTOFMEMORY;	}

	StringCchCopy( atName, MAX_PATH, ptName );
	PathStripPath( atName );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask = TCIF_TEXT;
	stTcItem.pszText = atName;
	TabCtrl_SetItem( ghFileTabWnd, iRslt, &stTcItem );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J���Ă�^�u��ۑ��������т�������
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	iTgt	�ǂݏo���ԍ��E�O�C���f�b�N�X�E�����Ȃ�t�@�C�����̂݊m��
	@param[out]	ptFile	�t���p�X�EMAX_PATH�ł��邱��
	@return		INT	�Z�[�u�F�O�@���[�h�F�L�^��
*/
INT InitMultiFileTabOpen( UINT dMode, INT iTgt, LPTSTR ptFile )
{
	if( dMode )	//	��т���
	{
		return DocMultiFileFetch( iTgt, ptFile, gatIniPath );
	}
	else	//	�Z�[�u
	{
		DocMultiFileStore( gatIniPath );
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J���Ă�^�u�����Commando
	@param[in]	iSelTab	�������^�u�ԍ��E�f�t�H����Ȃ�|�P
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MultiFileTabClose( INT iSelTab )
{
	INT	curSel, nowSel;
	LPARAM	dSele;
	TCITEM	stTcItem;

	nowSel = TabCtrl_GetCurSel( ghFileTabWnd );

	if( 0 > iSelTab ){	curSel = nowSel;	}	//	�I�����Ă�z���폜����
	else{				curSel = iSelTab;	}	//	�w��̃^�u���폜����

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask = TCIF_PARAM;
	TabCtrl_GetItem( ghFileTabWnd, curSel, &stTcItem );

	dSele = DocMultiFileClose( ghMainWnd, stTcItem.lParam );
	if( dSele )	//	�l�́A�^�u�ԍ��ł͂Ȃ��t�@�C���̒ʂ��ԍ��ł��邱�Ƃɒ���
	{
		TabCtrl_DeleteItem( ghFileTabWnd, curSel );
		//	�J���Ă�̂���������A�ׂ̂ɊJ������
		if( curSel ==  nowSel ){	MultiFileTabSelect( dSele );	}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�\���p�t�H���g�̖��O�𒸂�
*/
HRESULT ViewingFontNameLoad( VOID )
{
	TCHAR	atName[LF_FACESIZE];

	ZeroMemory( atName, sizeof(atName) );	//	�f�t�H�l�[��
	StringCchCopy( atName, LF_FACESIZE, TEXT("�l�r �o�S�V�b�N") );

	InitParamString( INIT_LOAD, VS_FONT_NAME, atName );	//	�Q�b�c�I

	StringCchCopy( gstBaseFont.lfFaceName, LF_FACESIZE, atName );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�\���p�t�H���g�f�[�^���R�s�[����
	@param[in]	pstLogFont	�f�[�^�R�s��\���̂ւ̃|�C���^�[
*/
HRESULT ViewingFontGet( LPLOGFONT pstLogFont )
{
	ZeroMemory( pstLogFont, sizeof(LOGFONT) );	//	�O�̂��ߋ󔒂ɂ���

	*pstLogFont = gstBaseFont;
	//	�\���̂̓R�s�[�ł���
	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#ifdef BIG_TEXT_SEPARATE
/*!
	����t�@�C�����J���Ƃ��̏����ɂ��Ė⍇���_�C�����O
	@param[in]	dStyle	���b�Z�[�W�^�C�v�E�P����t�@�C���⍇���@�Q���s�����G���[
	@return	�O�Ȃɂ����Ȃ��@�P�������[�h�@�Q�Ǎ����~
*/
UINT DocHugeFileTreatment( UINT dStyle )
{
	 INT	iRslt = IDNO;
	UINT	dMode;
	BOOL	bVisible;

	bVisible = IsWindowVisible( ghMainWnd );

	if( 1 == dStyle )
	{
		iRslt = MessageBox( bVisible ? ghMainWnd : NULL, TEXT("�傫�ȃt�@�C�����J�����Ƃ��Ă��B�K���ɕ������ēǂݍ��ނ����H\r\n\r\n�@�@�͂��F�����Ǎ��i�����ۑ����Ăˁj\t\n�@�@�������F���̂܂ܓǍ��i���Ԃ����邩���j\r\n�@�@�L�����Z���F�Ǎ����~"), TEXT("�ꖇ�̃e�L�X�g���ۂ���"), MB_YESNOCANCEL | MB_ICONQUESTION );
		//	�L���ɂȂ��ĂȂ��E�C���h�E�n���h����e�ɂ����ƂԂ��Ƃ�

		switch( iRslt )
		{
			case IDYES:	dMode = 1;	gbSeparateMod = TRUE;	break;	//	����
			case IDNO:	dMode = 0;	break;	//	�Ȃɂ����Ȃ�
			default:	dMode = 2;	break;	//	�Ǎ����~
		}
	}
	else if( 2 == dStyle )
	{
		MessageBox( bVisible ? ghMainWnd : NULL, TEXT("�t�@�C���̒��g���Ȃ񂩂���������B�Ǎ��𒆎~�����B"), TEXT("���s���Ȃ�����"), MB_OK | MB_ICONERROR );
		dMode = 2;
	}
	else
	{
		dMode = 2;
	}

	return dMode;
}
//-------------------------------------------------------------------------------------------------
#endif

#ifdef USE_NOTIFYICON

/*!
	�^�X�N�g���C�A�C�R���ŃC�x���g������������
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	uID		�C�x���g�����������g���C�A�C�R���̂h�c
	@param[in]	message	�}�E�X�A�L�[�{�[�h�̑����WMmessage
	@return ���ɂȂ�
*/
VOID TaskTrayIconEvent( HWND hWnd, UINT uID, UINT message )
{


	return;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	�^�X�N�g���C�A�C�R���ɂ΂��`��߂��[�����ڂ���
	@param[in]	ptInfo	�o���[���̖{��
	@param[in]	ptTitle	�o���[���̃^�C�g��
	@param[in]	dIconTy	�������A�C�R���A�P���A�Q�x���A�R�G���[
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT NotifyBalloonExist( LPTSTR ptInfo, LPTSTR ptTitle, DWORD dIconTy )
{
#ifdef USE_NOTIFYICON
	NOTIFYICONDATA	nid;

	ZeroMemory( &nid, sizeof(NOTIFYICONDATA) );
	nid.cbSize      = sizeof(NOTIFYICONDATA);
	nid.uFlags      = NIF_INFO;
	nid.hWnd        = ghMainWnd;
	StringCchCopy( nid.szInfoTitle, 64, ptTitle );
	StringCchCopy( nid.szInfo, 256, ptInfo );
	nid.uTimeout    = 15000;	//	�����łP�T�b�\�������Ă݂�
	nid.dwInfoFlags = dIconTy;	//	�p����A�C�R���^�C�v
	Shell_NotifyIcon( NIM_MODIFY, &nid );

//	NIIF_INFO       0x00000001
//	NIIF_WARNING    0x00000002
//	NIIF_ERROR      0x00000003
#endif
	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h�b�L���O���Ă��s�u���V�e���v����\��/��\��
*/
HRESULT DockingTmplViewToggle( UINT bMode )
{
	HWND	hDockWnd;
	RECT	clntRect, tabRect, pageRect, sbRect;
	RECT	rect;
	INT		iHeight, curSel;

	hDockWnd = DockingTabGet( );
	//	������ԂȂ�^�u�͖���
	if( !(hDockWnd) )	return E_ABORT;

	GetClientRect( ghMainWnd, &clntRect );
	GetClientRect( hDockWnd,  &tabRect );
	GetClientRect( ghPgVwWnd, &pageRect );

	GetClientRect( ghStsBarWnd, &sbRect );	//	�X�e�[�^�X�o�[�̃T�C�Y�m�F

	AppClientAreaCalc( &rect );

	if( gbDockTmplView )	//	�����Ă�Ȃ����΂���
	{
		gbDockTmplView = FALSE;
		ShowWindow( ghLnTmplWnd, SW_HIDE );
		ShowWindow( ghBrTmplWnd, SW_HIDE );

		//	�ꏊ��ς��Ȃ��Ƃ�����
		iHeight = clntRect.bottom - tabRect.bottom - sbRect.bottom;

		//	��s�u���V�^�u�͉���
		SetWindowPos( hDockWnd, HWND_TOP, rect.right - tabRect.right, iHeight, 0, 0, SWP_NOSIZE | SWP_NOZORDER );

		//	Page���X�g�͍L����
		SetWindowPos( ghPgVwWnd, HWND_TOP, 0, 0, pageRect.right, iHeight - rect.top, SWP_NOMOVE | SWP_NOZORDER );
	}
	else
	{
		gbDockTmplView = TRUE;

		//	�ʒu���킹
		ViewSizeMove( ghMainWnd, &rect );	//	�h�b�L���O���Ă��s�u���V�e���v����\��/��\��

		//	���̏�Ԃɍ��킹�ĕ��A
		curSel = TabCtrl_GetCurSel( hDockWnd );
		switch( curSel )
		{
			case  0:	ShowWindow( ghLnTmplWnd , SW_SHOW );	break;
			case  1:	ShowWindow( ghBrTmplWnd , SW_SHOW );	break;
			default:	break;
		}

	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#if defined(_DEBUG) || defined(WORK_LOG_OUT)
VOID OutputDebugStringPlus( DWORD rixError, LPTSTR ptFile, INT rdLine, LPTSTR ptFunc, LPTSTR ptFormat, ... )
//VOID OutputDebugStringPlus( DWORD rixError, LPSTR pcFile, INT rdLine, LPSTR pcFunc, LPTSTR ptFormat, ... )
{
	va_list	argp;
	TCHAR	atBuf[MAX_PATH], atOut[MAX_PATH], atFiFu[MAX_PATH], atErrMsg[MAX_PATH];
//	CHAR	acFile[MAX_PATH], acFiFu[MAX_PATH];
//	UINT	length;
#ifdef WORK_LOG_OUT
	UINT_PTR	cchLen;
	DWORD	wrote;
#endif

	StringCchCopy( atFiFu, MAX_PATH, ptFile );
	PathStripPath( atFiFu );
//	StringCchCopyA( acFile, MAX_PATH, pcFile );
//	PathStripPathA( acFile );

//	StringCchPrintfA( acFiFu, MAX_PATH, ("%s %d %s"), acFile, rdLine, pcFunc );
//	length = (UINT)strlen( acFiFu );

//	ZeroMemory( atFiFu, sizeof(atFiFu) );
//	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, acFiFu, length, atFiFu, MAX_PATH );
	//	�R�[�h�y�[�W,�����̎�ނ��w�肷��t���O,�}�b�v��������̃A�h���X,�}�b�v��������̃o�C�g��,
	//	�}�b�v�惏�C�h�����������o�b�t�@�̃A�h���X,�o�b�t�@�̃T�C�Y

	va_start(argp, ptFormat);
	StringCchVPrintf( atBuf, MAX_PATH, ptFormat, argp );
	va_end( argp );

	StringCchPrintf( atOut, MAX_PATH, TEXT("%s @ %s %d %s\r\n"), atBuf, atFiFu, rdLine, ptFunc );//
//	StringCchPrintf( atOut, MAX_PATH, TEXT("%s @ %s\r\n"), atBuf, atFiFu );//

#ifdef _DEBUG
	OutputDebugString( atOut );
#endif

#ifdef WORK_LOG_OUT
	if( INVALID_HANDLE_VALUE != ghLogFlie )
	{
		StringCchLength( atOut, MAX_PATH, &cchLen );
		WriteFile( ghLogFlie, atOut, cchLen * sizeof(TCHAR), &wrote, NULL );
	}
#endif

	if( rixError )
	{
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, rixError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), atErrMsg, MAX_PATH, NULL );
		//	���b�Z�[�W�ɂ͉��s���܂܂�Ă���悤��
		StringCchPrintf( atBuf, MAX_PATH, TEXT("[%d]%s"), rixError, atErrMsg );//

#ifdef _DEBUG
		OutputDebugString( atBuf );
#endif
#ifdef WORK_LOG_OUT
		if( INVALID_HANDLE_VALUE != ghLogFlie )
		{
			StringCchLength( atBuf, MAX_PATH, &cchLen );
			WriteFile( ghLogFlie, atBuf, cchLen * sizeof(TCHAR), &wrote, NULL );
		}
#endif

		SetLastError( 0 );
	}
}
//-------------------------------------------------------------------------------------------------
#endif

//	Dirty Deeds Done Dirt Cheap �����̃X�^���h�Ɂu���Ƃ��e�Ղ��s���邦���Ȃ��s�ׁv�Ȃ�Ė��O��t����̂͂ǂ����Ǝv���B
