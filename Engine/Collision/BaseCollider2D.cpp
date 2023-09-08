#include "BaseCollider2D.h"

//const Vector2& BaseCollider2D::GetPosition()
//{
//    if (sprite_) {
//        return sprite_->GetPosition();
//    }
//
//    return Vector2(0.0f, 0.0f);
//}

Vector2 BaseCollider2D::GetPosition() const
{
    if (sprite_) {
        return sprite_->GetPosition();
    }

    return Vector2(0.0f, 0.0f);
}
