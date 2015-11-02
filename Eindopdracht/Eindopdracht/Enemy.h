#ifndef __ENEMY_H__
#define __ENEMY_H__

class Enemy
{
	friend class EnemyGenerator;
	public:

		Enemy(std::string type, std::string size, int level, int maxHP, int chanceToHit, int chanceToDefend, int attack, int chanceHeroEscapes);
		~Enemy();

		bool getAttackedByHero(int damage);

		bool getIsDefeated();
		
		std::string getType() const;
		std::string getSize() const;
		int getCurrentHP();
		int getChanceToHit();
		int getAttack();
		int getChanceHeroEscapes();
		int getXP();

	private:

		bool isDefeated_;

		std::string type_;
		std::string size_;
		
		int level_;
		int maxHP_;
		int currentHP_;
		int chanceToHit_;
		int chanceToDefend_;
		int attack_;
		int chanceHeroEscapes_;
};

std::ostream& operator<<(std::ostream& os, const Enemy& obj);

#endif

